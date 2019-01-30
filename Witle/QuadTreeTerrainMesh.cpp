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

	if (nWidth <= m_widthMin || nLength <= m_lengthMin) // 마지막 리프 노드
	{
		// 터레인을 생성한다.
		node->terrainMesh = new TerrainMesh(m_pOwner, pd3dDevice, pd3dCommandList, xStart, zStart, nWidth, nLength, m_xmf3Scale, m_xmf4Color, pContext);
	}
	else
	{
		// 자식을 만들어야 한다.
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
	// 사례 1: 이 노드에 삼각형이 없으면 비어있는 상태로 돌아가서 처리할 필요가 없습니다.
	if (numTriangles == 0) return true;

	// 사례 2: 이 노드에 너무 많은 삼각형이 있는 경우 4 개의 동일한 크기의 더 작은 트리 노드로 분할합니다.
	else if (numTriangles > MAX_TRIANGLES)
	{
		for (int i = 0; i < 4; i++)
		{
			// 새로운 자식 노드에 대한 위치 오프셋을 계산합니다.
			// float offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			// float offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			// 이 새 노드가있는 삼각형이있는 경우 자식 노드를 만듭니다.
			node->nodes[i] = new NODE_TYPE;

			// 이제 이 새 자식 노드에서 시작하는 트리를 확장합니다.
			// CreateTreeNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f), device);
		}
		return true;
	}

	return false;
}

void QuadTreeTerrainMesh::CreateTreeNode(NODE_TYPE* node, float positionX, float positionZ, float width, float length, ID3D12Device* device)
{
	// 노드의 위치와 크기를 저장한다.
	node->positionX = positionX;
	node->positionZ = positionZ;
	node->width = width;
	node->length = length;

	// 초기화
	node->triangleCount = 0;
	node->vertexBuffer = nullptr;
	node->indexBuffer = nullptr;
	for (auto& node : node->nodes)
	{
		node = nullptr;
	} 

	// 이 노드 안에 있는 삼각형 수를 센다.
	int numTriangles = CalculateTriangles(width, length);

	// 만약 삼각형수가 ...
	bool isOk = IsCheckTriangleCount(node, numTriangles);
	if (isOk) return;

	node->triangleCount = numTriangles;

	// 정점의 수를 계산합니다.
	int vertexCount = width * length;
	CDiffused2TexturedVertex* vertices = new CDiffused2TexturedVertex[vertexCount];
	unsigned long* indices = new unsigned long[vertexCount + 10]; // 실제로 vertex보다 index개수가 더 많음.

	// 이 새로운 정점 및 인덱스 배열의 인덱스를 초기화합니다.
	
	// 정점 목록의 모든 삼각형을 집어 넣습니다.
	//for (int i = 0; i < m_triangleCount; i++)
	//{
	//	// 정점 목록에서 이 삼각형의 세 꼭지점을 가져옵니다.
	//	vertices[i].m_xmf3Position = m_vertexList[vertexIndex].m_xmf3Position; 
	//}

	//// 정점 버퍼의 구조체를 설정합니다.
	//D3D12_BUFFER_DESC vertexBufferDesc;
	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(CDiffused2TexturedVertex) * vertexCount;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;
	//vertexBufferDesc.StructureByteStride = 0;

	//// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	//D3D11_SUBRESOURCE_DATA vertexData;
	//vertexData.pSysMem = vertices;
	//vertexData.SysMemPitch = 0;
	//vertexData.SysMemSlicePitch = 0;

	//// 이제 마침내 정점 버퍼를 만듭니다.
	//device->CreateBuffer(&vertexBufferDesc, &vertexData, &node->vertexBuffer);

	//// 인덱스 버퍼의 설명을 설정합니다.
	//D3D11_BUFFER_DESC indexBufferDesc;
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
	//D3D11_SUBRESOURCE_DATA indexData;
	//indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// 인덱스 버퍼를 만듭니다.
	//device->CreateBuffer(&indexBufferDesc, &indexData, &node->indexBuffer);

	// 이제 노드의 버퍼에 데이터가 저장되므로 꼭지점과 인덱스 배열을 해제합니다.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void QuadTreeTerrainMesh::ReleaseNode(NODE_TYPE * node)
{
	// 재귀 적으로 트리 아래로 내려와 맨 아래 노드를 먼저 놓습니다.
	for (int i = 0; i < 4; i++)
	{
		if (node->nodes[i] != 0)
		{
			ReleaseNode(node->nodes[i]);
		}
	}

	// 이 노드의 버텍스 버퍼를 해제한다.
	if (node->vertexBuffer)
	{
		node->vertexBuffer->Release();
		node->vertexBuffer = 0;
	}

	// 이 노드의 인덱스 버퍼를 해제합니다.
	if (node->indexBuffer)
	{
		node->indexBuffer->Release();
		node->indexBuffer = 0;
	}

	// 네 개의 자식 노드를 해제합니다.
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

	// 쿼드 트리의 부모 노드를 만듭니다.
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
