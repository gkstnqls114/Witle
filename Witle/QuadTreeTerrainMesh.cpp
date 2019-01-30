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
 
INFO QuadTreeTerrainMesh::CalculateMeshDimensions(int vertexCount)
{
	// 메쉬의 중심 위치를 0으로 초기화합니다.
	float centerX = 0.0f;
	float centerZ = 0.0f;

	// 메쉬의 모든 정점을 합친다.
	for (int i = 0; i < vertexCount; i++)
	{
		centerX += m_vertexList[i].m_xmf3Position.x;
		centerZ += m_vertexList[i].m_xmf3Position.z;
	}

	// 그리고 메쉬의 중간 점을 찾기 위해 정점의 수로 나눕니다.
	centerX = centerX / (float)vertexCount;
	centerZ = centerZ / (float)vertexCount;

	// 메쉬의 최대 및 최소 크기를 초기화합니다.
	float maxWidth = 0.0f;
	float maxDepth = 0.0f;

	float minWidth = fabsf(m_vertexList[0].m_xmf3Position.x - centerX);
	float minDepth = fabsf(m_vertexList[0].m_xmf3Position.z - centerZ);

	// 모든 정점을 살펴보고 메쉬의 최대 너비와 최소 너비와 깊이를 찾습니다.
	for (int i = 0; i < vertexCount; i++)
	{
		float width = fabsf(m_vertexList[i].m_xmf3Position.x - centerX);
		float depth = fabsf(m_vertexList[i].m_xmf3Position.z - centerZ);

		if (width > maxWidth) { maxWidth = width; }
		if (depth > maxDepth) { maxDepth = depth; }
		if (width < minWidth) { minWidth = width; }
		if (depth < minDepth) { minDepth = depth; }
	}

	// 최소와 최대 깊이와 너비 사이의 절대 최대 값을 찾습니다.
	float maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	float maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	// 메쉬의 최대 직경을 계산합니다.
	float meshWidth = max(maxX, maxZ) * 2.0f;

	return INFO{ centerX, centerZ, meshWidth };
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
	// 지형 정점 배열의 모든 정점 수를 가져옵니다.
	int vertexCount = pTerrain->GetVertexCount();
	int terrainWidth = pTerrain->GetWidth();
	int terrainlength = pTerrain->GetLength();
	assert(!(terrainWidth != terrainlength)); // width, length 가 같은 경우에만 사용

	// 정점리스트의 총 삼각형 수를 저장합니다.
	m_triangleCount = CalculateTriangles(terrainWidth - 1, terrainlength - 1); // width - 1 = 픽셀 width

	m_vertexList = new CDiffused2TexturedVertex[vertexCount];
	if (!m_vertexList) return false;
	 
	// 지형 정점을 정점 목록에 복사합니다.
	pTerrain->CopyVertexArray(SourcePtr(m_vertexList));

	// 중심 x, z와 메쉬의 너비를 계산합니다.
	INFO value = CalculateMeshDimensions(vertexCount);

	// 쿼드 트리의 부모 노드를 만듭니다.
	m_parentNode = new NODE_TYPE;
	if (!m_parentNode) return false;

	// 정점 목록 데이터와 메쉬 차원을 기반으로 쿼드 트리를 재귀 적으로 빌드합니다.
	CreateTreeNode(m_parentNode, value.centerX, value.centerZ, value.meshWidth, 0.f, pd3dDevice);

	// 쿼드 트리가 각 노드에 정점을 갖기 때문에 정점 목록을 놓습니다.
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
