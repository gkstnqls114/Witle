#include "stdafx.h"
#include "TerrainMesh.h" 
#include "QuadTreeTerrainMesh.h"

void QuadTreeTerrainMesh::CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth)
{
	// �޽��� �߽� ��ġ�� 0���� �ʱ�ȭ�մϴ�.
	centerX = 0.0f;
	centerZ = 0.0f;

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
	meshWidth = max(maxX, maxZ) * 2.0f;
}

void QuadTreeTerrainMesh::CreateTreeNode(NODE_TYPE* node, float positionX, float positionZ, float width, ID3D12Device* device)
{

	// ����� ��ġ�� ũ�⸦ �����Ѵ�.
	node->positionX = positionX;
	node->positionZ = positionZ;
	node->width = width;

	// ����� �ﰢ�� ���� 0���� �ʱ�ȭ�մϴ�.
	node->triangleCount = 0;

	//���� �� �ε��� ���۸� null�� �ʱ�ȭ�մϴ�.
	node->vertexBuffer = 0;
	node->indexBuffer = 0;

	// �� ����� �ڽ� ��带 null�� �ʱ�ȭ�մϴ�.
	node->nodes[0] = 0;
	node->nodes[1] = 0;
	node->nodes[2] = 0;
	node->nodes[3] = 0;

	// �� ��� �ȿ� �ִ� �ﰢ�� ���� ����.
	int numTriangles = CountTriangles(positionX, positionZ, width);

	// ��� 1: �� ��忡 �ﰢ���� ������ ����ִ� ���·� ���ư��� ó���� �ʿ䰡 �����ϴ�.
	if (numTriangles == 0)
	{
		return;
	}

	// ��� 2: �� ��忡 �ʹ� ���� �ﰢ���� �ִ� ��� 4 ���� ������ ũ���� �� ���� Ʈ�� ���� �����մϴ�.
	if (numTriangles > MAX_TRIANGLES)
	{
		for (int i = 0; i < 4; i++)
		{
			// ���ο� �ڽ� ��忡 ���� ��ġ �������� ����մϴ�.
			float offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			float offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			// �� ��忡 �ﰢ���� �ִ��� Ȯ���մϴ�.
			int count = CountTriangles((positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
			if (count > 0)
			{
				// �� �� ��尡�ִ� �ﰢ�����ִ� ��� �ڽ� ��带 ����ϴ�.
				node->nodes[i] = new NODE_TYPE;

				// ������ �� �ڽ� ��忡�� �����ϴ� Ʈ���� Ȯ���մϴ�.
				CreateTreeNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f), device);
			}
		}
		return;
	}

	// ��� 3: �� ��尡 ��� �����ʰ� �� ����� �ﰢ�� ���� �ִ� ������ ������ 
	// �� ���� Ʈ���� �� �Ʒ��� �����Ƿ� ������ �ﰢ�� ����� ����ϴ�.
	node->triangleCount = numTriangles;

	// ������ ���� ����մϴ�.
	int vertexCount = numTriangles * 3;

	// ���� �迭�� ����ϴ�.
	CDiffused2TexturedVertex* vertices = new CDiffused2TexturedVertex[vertexCount];

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[vertexCount];

	// �� ���ο� ���� �� �ε��� �迭�� �ε����� �ʱ�ȭ�մϴ�.
	int index = 0;
	int vertexIndex = 0;

	// ���� ����� ��� �ﰢ���� ���� ���ϴ�.
	for (int i = 0; i < m_triangleCount; i++)
	{
		// �ﰢ������ ��� �ȿ� ������ ������ �迭�� �߰��մϴ�.
		if (IsTriangleContained(i, positionX, positionZ, width) == true)
		{
			// ���� ���ؽ� ��Ͽ� �ε����� ����մϴ�.
			vertexIndex = i * 3;

			// ���� ��Ͽ��� �� �ﰢ���� �� �������� �����ɴϴ�.
			vertices[index].m_xmf3Position = m_vertexList[vertexIndex].m_xmf3Position;
			// vertices[index].texture = m_vertexList[vertexIndex].texture;
			// vertices[index].normal = m_vertexList[vertexIndex].normal;
			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index].m_xmf3Position = m_vertexList[vertexIndex].m_xmf3Position;
			// vertices[index].texture = m_vertexList[vertexIndex].texture;
			// vertices[index].normal = m_vertexList[vertexIndex].normal;
			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index].m_xmf3Position = m_vertexList[vertexIndex].m_xmf3Position;
			// vertices[index].texture = m_vertexList[vertexIndex].texture;
			// vertices[index].normal = m_vertexList[vertexIndex].normal;
			indices[index] = index;
			index++;
		}
	}

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

int QuadTreeTerrainMesh::CountTriangles(float positionX, float positionZ, float width)
{
	// ī��Ʈ�� 0���� �ʱ�ȭ�Ѵ�.
	int count = 0;

	// ��ü �޽��� ��� �ﰢ���� ���캸�� � ��尡 �� ��� �ȿ� �־�� �ϴ��� Ȯ���մϴ�.
	for (int i = 0; i < m_triangleCount; i++)
	{
		// �ﰢ���� ��� �ȿ� ������ 1�� ������ŵ�ϴ�.
		if (IsTriangleContained(i, positionX, positionZ, width) == true)
		{
			count++;
		}
	}

	return count;
}

bool QuadTreeTerrainMesh::IsTriangleContained(int index, float positionX, float positionZ, float width)
{
	// �� ����� �ݰ��� ����մϴ�.
	float radius = width / 2.0f;

	// �ε����� ���� ������� �����ɴϴ�.
	int vertexIndex = index * 3;

	// ���� ��Ͽ��� �� �ﰢ���� �� �������� �����ɴϴ�.
	float x1 = m_vertexList[vertexIndex].m_xmf3Position.x;
	float z1 = m_vertexList[vertexIndex].m_xmf3Position.z;
	vertexIndex++;

	float x2 = m_vertexList[vertexIndex].m_xmf3Position.x;
	float z2 = m_vertexList[vertexIndex].m_xmf3Position.z;
	vertexIndex++;

	float x3 = m_vertexList[vertexIndex].m_xmf3Position.x;
	float z3 = m_vertexList[vertexIndex].m_xmf3Position.z;

	// �ﰢ���� x ��ǥ�� �ּҰ��� ��� �ȿ� �ִ��� Ȯ���Ͻʽÿ�.
	float minimumX = min(x1, min(x2, x3));
	if (minimumX > (positionX + radius))
	{
		return false;
	}

	// �ﰢ���� x ��ǥ�� �ִ� ���� ��� �ȿ� �ִ��� Ȯ���Ͻʽÿ�.
	float maximumX = max(x1, max(x2, x3));
	if (maximumX < (positionX - radius))
	{
		return false;
	}

	// �ﰢ���� z ��ǥ�� �ּҰ��� ��� �ȿ� �ִ��� Ȯ���Ͻʽÿ�.
	float minimumZ = min(z1, min(z2, z3));
	if (minimumZ > (positionZ + radius))
	{
		return false;
	}

	// �ﰢ���� z ��ǥ�� �ִ� ���� ��� �ȿ� �ִ��� Ȯ���Ͻʽÿ�.
	float maximumZ = max(z1, max(z2, z3));
	if (maximumZ < (positionZ - radius))
	{
		return false;
	}

	return true;
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
	float centerX = 0.0f;
	float centerZ = 0.0f;
	float width = 0.0f;

	// ���� ���� �迭�� ���� ���� �����ɴϴ�.
	int vertexCount = pTerrain->GetVertexCount();

	// ��������Ʈ�� �� �ﰢ�� ���� �����մϴ�.
	m_triangleCount = vertexCount / 3;

	// ��� ���� ������ �����ϴ� ���� �迭�� ����ϴ�.
	m_vertexList = new CDiffused2TexturedVertex[vertexCount];
	if (!m_vertexList)
	{
		return false;
	}

	// ���� ������ ���� ��Ͽ� �����մϴ�.
	pTerrain->CopyVertexArray(SourcePtr(m_vertexList));

	// �߽� x, z�� �޽��� �ʺ� ����մϴ�.
	CalculateMeshDimensions(vertexCount, centerX, centerZ, width);

	// ���� Ʈ���� �θ� ��带 ����ϴ�.
	m_parentNode = new NODE_TYPE;
	if (!m_parentNode)
	{
		return false;
	}

	// ���� ��� �����Ϳ� �޽� ������ ������� ���� Ʈ���� ��� ������ �����մϴ�.
	CreateTreeNode(m_parentNode, centerX, centerZ, width, pd3dDevice);

	// ���� Ʈ���� �� ��忡 ������ ���� ������ ���� ����� �����ϴ�.
	if (m_vertexList)
	{
		delete[] m_vertexList;
		m_vertexList = 0;
	}
	return false;
}
