#include "stdafx.h"
#include "d3dUtil.h"
#include "TerrainMesh.h" 
#include "HeightMapImage.h"
#include "QuadTreeTerrainMesh.h"

UINT QuadTreeTerrainMesh::CalculateTriangles(UINT widthPixel, UINT lengthPixel)
{
	assert(!(widthPixel != lengthPixel));
	
	return (widthPixel - 1) * (lengthPixel -1) * 2;
}

void QuadTreeTerrainMesh::RecursiveCreateTerrain(QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, HeightMapImage * pContext)
{
	assert(!(nWidth < 3));
	assert(!(nWidth / 2 != 0));
	assert(!(nLength < 3));
	assert(!(nLength / 2 != 0));

	if (nWidth <= m_widthMin || nLength <= m_lengthMin) // ������ ���� ���
	{
		// �ͷ����� �����Ѵ�.
		node->terrainMesh = new TerrainMesh(m_pOwner, pd3dDevice, pd3dCommandList, xStart, zStart, nWidth, nLength, m_xmf3Scale, m_xmf4Color, pContext);
	}
	else
	{
		// �ڽ��� ������ �Ѵ�.
		for (int z = 0, zStart = 0; z < czBlocks; z++)
		{
			for (int x = 0, xStart = 0; x < cxBlocks; x++)
			{
				xStart = x * (nBlockWidth - 1);
				zStart = z * (nBlockLength - 1);
			}
		}
		for (int x = 0; x < QUAD; ++x)
		{
			node->nodes[x] = new QUAD_TREE_NODE();
			RecursiveCreateTerrain(node->nodes[x], pd3dDevice, pd3dCommandList, xStart, zStart, nWidth / 2, nLength / 2, pContext);
		}
	}
}

UINT QuadTreeTerrainMesh::CalculateVertex(UINT widht, UINT length)
{
	return (widht* length);
}

bool QuadTreeTerrainMesh::IsCheckTriangleCount(NODE_TYPE* node, UINT numTriangles)
{
	// ��� 1: �� ��忡 �ﰢ���� ������ ����ִ� ���·� ���ư��� ó���� �ʿ䰡 �����ϴ�.
	if (numTriangles == 0) return true;

	// ��� 2: �� ��忡 �ʹ� ���� �ﰢ���� �ִ� ��� 4 ���� ������ ũ���� �� ���� Ʈ�� ���� �����մϴ�.
	else if (numTriangles > MAX_TRIANGLES)
	{
		for (int i = 0; i < 4; i++)
		{
			// ���ο� �ڽ� ��忡 ���� ��ġ �������� ����մϴ�.
			// float offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			// float offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			// �� �� ��尡�ִ� �ﰢ�����ִ� ��� �ڽ� ��带 ����ϴ�.
			node->nodes[i] = new NODE_TYPE;

			// ���� �� �� �ڽ� ��忡�� �����ϴ� Ʈ���� Ȯ���մϴ�.
			// CreateTreeNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f), device);
		}
		return true;
	}

	return false;
}

void QuadTreeTerrainMesh::CreateTreeNode(NODE_TYPE* node, float positionX, float positionZ, float width, float length, ID3D12Device* device)
{
	// ����� ��ġ�� ũ�⸦ �����Ѵ�.
	node->positionX = positionX;
	node->positionZ = positionZ;
	node->width = width;
	node->length = length;

	// �ʱ�ȭ
	node->triangleCount = 0;
	node->vertexBuffer = nullptr;
	node->indexBuffer = nullptr;
	for (auto& node : node->nodes)
	{
		node = nullptr;
	} 

	// �� ��� �ȿ� �ִ� �ﰢ�� ���� ����.
	int numTriangles = CalculateTriangles(width, length);

	// ���� �ﰢ������ ...
	bool isOk = IsCheckTriangleCount(node, numTriangles);
	if (isOk) return;

	node->triangleCount = numTriangles;

	// ������ ���� ����մϴ�.
	int vertexCount = width * length;
	CDiffused2TexturedVertex* vertices = new CDiffused2TexturedVertex[vertexCount];
	unsigned long* indices = new unsigned long[vertexCount + 10]; // ������ vertex���� index������ �� ����.

	// �� ���ο� ���� �� �ε��� �迭�� �ε����� �ʱ�ȭ�մϴ�.
	
	// ���� ����� ��� �ﰢ���� ���� �ֽ��ϴ�.
	//for (int i = 0; i < m_triangleCount; i++)
	//{
	//	// ���� ��Ͽ��� �� �ﰢ���� �� �������� �����ɴϴ�.
	//	vertices[i].m_xmf3Position = m_vertexList[vertexIndex].m_xmf3Position; 
	//}

	//// ���� ������ ����ü�� �����մϴ�.
	//D3D12_BUFFER_DESC vertexBufferDesc;
	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(CDiffused2TexturedVertex) * vertexCount;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;
	//vertexBufferDesc.StructureByteStride = 0;

	//// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	//D3D11_SUBRESOURCE_DATA vertexData;
	//vertexData.pSysMem = vertices;
	//vertexData.SysMemPitch = 0;
	//vertexData.SysMemSlicePitch = 0;

	//// ���� ��ħ�� ���� ���۸� ����ϴ�.
	//device->CreateBuffer(&vertexBufferDesc, &vertexData, &node->vertexBuffer);

	//// �ε��� ������ ������ �����մϴ�.
	//D3D11_BUFFER_DESC indexBufferDesc;
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� �����͸� �����մϴ�.
	//D3D11_SUBRESOURCE_DATA indexData;
	//indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// �ε��� ���۸� ����ϴ�.
	//device->CreateBuffer(&indexBufferDesc, &indexData, &node->indexBuffer);

	// ���� ����� ���ۿ� �����Ͱ� ����ǹǷ� �������� �ε��� �迭�� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void QuadTreeTerrainMesh::ReleaseNode(NODE_TYPE * node)
{
	// ��� ������ Ʈ�� �Ʒ��� ������ �� �Ʒ� ��带 ���� �����ϴ�.
	for (int i = 0; i < 4; i++)
	{
		if (node->nodes[i] != 0)
		{
			ReleaseNode(node->nodes[i]);
		}
	}

	// �� ����� ���ؽ� ���۸� �����Ѵ�.
	if (node->vertexBuffer)
	{
		node->vertexBuffer->Release();
		node->vertexBuffer = 0;
	}

	// �� ����� �ε��� ���۸� �����մϴ�.
	if (node->indexBuffer)
	{
		node->indexBuffer->Release();
		node->indexBuffer = 0;
	}

	// �� ���� �ڽ� ��带 �����մϴ�.
	for (int i = 0; i < 4; i++)
	{
		if (node->nodes[i])
		{
			delete node->nodes[i];
			node->nodes[i] = 0;
		}
	}
}

QuadTreeTerrainMesh::QuadTreeTerrainMesh(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, HeightMapImage * pContext)
	: ComponentBase(pOwner)
{
	m_ComponenetID = MESH_TYPE_ID::QUADTREE_TERRAIN_MESH;

	m_widthTotal = nWidth;
	m_lengthTotal = nLength;
	m_xmf3Scale = xmf3Scale;
	m_xmf4Color = xmf4Color;

	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	// ���� Ʈ���� �θ� ��带 ����ϴ�.
	m_pRootNode = new QUAD_TREE_NODE;


	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	TerrainMesh *pterrainMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			pterrainMesh = new TerrainMesh(m_pOwner, pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, pHeightMapImage);
			// SetMesh(x + (z*cxBlocks), pHeightMapGridMesh);
		}
	}

	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_widthTotal, pContext);
}

QuadTreeTerrainMesh::~QuadTreeTerrainMesh()
{

}

bool QuadTreeTerrainMesh::Initialize(TerrainMesh * pTerrain, ID3D12Device * pd3dDevice)
{

}

void QuadTreeTerrainMesh::ReleaseUploadBuffers()
{
}
