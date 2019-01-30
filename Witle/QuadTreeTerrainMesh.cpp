#include "stdafx.h"
#include "TerrainMesh.h" 
#include "QuadTreeTerrainMesh.h"

UINT QuadTreeTerrainMesh::CalculateTriangles(UINT widthPixel, UINT lengthPixel)
{
	assert(!(widthPixel != lengthPixel));
	
	return (widthPixel - 1) * (lengthPixel -1) * 2;
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
 
INFO QuadTreeTerrainMesh::CalculateMeshDimensions(int vertexCount)
{
	// �޽��� �߽� ��ġ�� 0���� �ʱ�ȭ�մϴ�.
	float centerX = 0.0f;
	float centerZ = 0.0f;

	// �޽��� ��� ������ ��ģ��.
	for (int i = 0; i < vertexCount; i++)
	{
		centerX += m_vertexList[i].m_xmf3Position.x;
		centerZ += m_vertexList[i].m_xmf3Position.z;
	}

	// �׸��� �޽��� �߰� ���� ã�� ���� ������ ���� �����ϴ�.
	centerX = centerX / (float)vertexCount;
	centerZ = centerZ / (float)vertexCount;

	// �޽��� �ִ� �� �ּ� ũ�⸦ �ʱ�ȭ�մϴ�.
	float maxWidth = 0.0f;
	float maxDepth = 0.0f;

	float minWidth = fabsf(m_vertexList[0].m_xmf3Position.x - centerX);
	float minDepth = fabsf(m_vertexList[0].m_xmf3Position.z - centerZ);

	// ��� ������ ���캸�� �޽��� �ִ� �ʺ�� �ּ� �ʺ�� ���̸� ã���ϴ�.
	for (int i = 0; i < vertexCount; i++)
	{
		float width = fabsf(m_vertexList[i].m_xmf3Position.x - centerX);
		float depth = fabsf(m_vertexList[i].m_xmf3Position.z - centerZ);

		if (width > maxWidth) { maxWidth = width; }
		if (depth > maxDepth) { maxDepth = depth; }
		if (width < minWidth) { minWidth = width; }
		if (depth < minDepth) { minDepth = depth; }
	}

	// �ּҿ� �ִ� ���̿� �ʺ� ������ ���� �ִ� ���� ã���ϴ�.
	float maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	float maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	// �޽��� �ִ� ������ ����մϴ�.
	float meshWidth = max(maxX, maxZ) * 2.0f;

	return INFO{ centerX, centerZ, meshWidth };
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

QuadTreeTerrainMesh::QuadTreeTerrainMesh(GameObject* pOwner)
	: Mesh(pOwner)
{
	m_ComponenetID = MESH_TYPE_ID::QUADTREE_TERRAIN_MESH;

}

QuadTreeTerrainMesh::QuadTreeTerrainMesh(const QuadTreeTerrainMesh & other)
	:Mesh(other.m_pOwner)
{

}

QuadTreeTerrainMesh::~QuadTreeTerrainMesh()
{

}

bool QuadTreeTerrainMesh::Initialize(TerrainMesh * pTerrain, ID3D12Device * pd3dDevice)
{
	// ���� ���� �迭�� ��� ���� ���� �����ɴϴ�.
	int vertexCount = pTerrain->GetVertexCount();
	int terrainWidth = pTerrain->GetWidth();
	int terrainlength = pTerrain->GetLength();
	assert(!(terrainWidth != terrainlength)); // width, length �� ���� ��쿡�� ���

	// ��������Ʈ�� �� �ﰢ�� ���� �����մϴ�.
	m_triangleCount = CalculateTriangles(terrainWidth - 1, terrainlength - 1); // width - 1 = �ȼ� width

	m_vertexList = new CDiffused2TexturedVertex[vertexCount];
	if (!m_vertexList) return false;
	 
	// ���� ������ ���� ��Ͽ� �����մϴ�.
	pTerrain->CopyVertexArray(SourcePtr(m_vertexList));

	// �߽� x, z�� �޽��� �ʺ� ����մϴ�.
	INFO value = CalculateMeshDimensions(vertexCount);

	// ���� Ʈ���� �θ� ��带 ����ϴ�.
	m_parentNode = new NODE_TYPE;
	if (!m_parentNode) return false;

	// ���� ��� �����Ϳ� �޽� ������ ������� ���� Ʈ���� ��� ������ �����մϴ�.
	CreateTreeNode(m_parentNode, value.centerX, value.centerZ, value.meshWidth, 0.f, pd3dDevice);

	// ���� Ʈ���� �� ��忡 ������ ���� ������ ���� ����� �����ϴ�.
	if (m_vertexList)
	{
		delete[] m_vertexList;
		m_vertexList = 0;
	}
	return false;
}

void QuadTreeTerrainMesh::ReleaseUploadBuffers()
{
}
