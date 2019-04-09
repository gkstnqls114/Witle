#include "stdafx.h"
#include "d3dUtil.h"
#include "TerrainMesh.h" 
#include "ShaderManager.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "StaticObjectStorage.h"
#include "HeightMapImage.h"
#include "MyFrustum.h"
#include "QuadTreeTerrainMesh.h"

// ó�� ���̵�� 0���� �����Ѵ�.
// ���� ��常 ���̵� �����Ѵ�.
// ���� ���̵�� 0���� �����Ѵ�.
int QuadtreeTerrain::gTreePieceCount{ 0 }; 

UINT QuadtreeTerrain::CalculateTriangles(UINT widthPixel, UINT lengthPixel)
{
	assert(!(widthPixel != lengthPixel));
	
	return (widthPixel - 1) * (lengthPixel -1) * 2;
}
  

void QuadtreeTerrain::ReleaseMembers()
{
	delete[] m_pReafNodes;
	delete[] m_RenderingIndices;
}

void QuadtreeTerrain::ReleaseMemberUploadBuffers()
{ 
	RecursiveReleaseUploadBuffers(m_pRootNode);
}

void QuadtreeTerrain::RenderTerrainObjects(ID3D12GraphicsCommandList * pd3dCommandList)
{
	StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, m_RenderingIndices, m_ReafNodeCount);
}

void QuadtreeTerrain::RecursiveRender(const QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList)
{
	// ������
	extern MeshRenderer gMeshRenderer;

	if (node->terrainMesh)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
		gMeshRenderer.Render(pd3dCommandList, node->terrainMesh); 
	}
	else
	{
		if (node->children[0]->isRendering) RecursiveRender(node->children[0], pd3dCommandList);
		if (node->children[1]->isRendering) RecursiveRender(node->children[1], pd3dCommandList);
		if (node->children[2]->isRendering) RecursiveRender(node->children[2], pd3dCommandList);
		if (node->children[3]->isRendering) RecursiveRender(node->children[3], pd3dCommandList);
	}
}

void QuadtreeTerrain::RecursiveInitReafNodes(QUAD_TREE_NODE * node)
{ 
	if (node->terrainMesh)
	{
		m_pReafNodes[node->id] = node;
	}
	else
	{
		RecursiveInitReafNodes(node->children[0]);
		RecursiveInitReafNodes(node->children[1]);
		RecursiveInitReafNodes(node->children[2]);
		RecursiveInitReafNodes(node->children[3]);
	}
}

void QuadtreeTerrain::RecursiveReleaseUploadBuffers(QUAD_TREE_NODE * node)
{
	if (node->terrainMesh)
	{
		node->terrainMesh->ReleaseUploadBuffers();
	}
	else
	{
		RecursiveReleaseUploadBuffers(node->children[0]);
		RecursiveReleaseUploadBuffers(node->children[1]);
		RecursiveReleaseUploadBuffers(node->children[2]);
		RecursiveReleaseUploadBuffers(node->children[3]);
	}
}

void QuadtreeTerrain::RecursiveReleaseObjects(QUAD_TREE_NODE * node)
{
	if (node->terrainMesh)
	{
		delete node->terrainMesh;
		node->terrainMesh = nullptr;
	}
	else
	{
		RecursiveReleaseObjects(node->children[0]);
		RecursiveReleaseObjects(node->children[1]);
		RecursiveReleaseObjects(node->children[2]);
		RecursiveReleaseObjects(node->children[3]);

		delete node->children[0];
		delete node->children[1];
		delete node->children[2];
		delete node->children[3];

		node->children[0] = nullptr;
		node->children[1] = nullptr;
		node->children[2] = nullptr;
		node->children[3] = nullptr;
	}
}

void QuadtreeTerrain::RecursiveCalculateIDs(QUAD_TREE_NODE * node, const XMFLOAT3 position, int* pIDs) const
{
	if (node->terrainMesh)
	{
		assert(!(node->id == -1));
		// �������� �ش� �޽��� �´��� Ȯ���Ѵ�. 
		// x, z ���̿� �ִ��� �˻��Ѵ�.
		float minX = node->boundingBox.Center.x - node->boundingBox.Extents.x;
		float maxX = node->boundingBox.Center.x + node->boundingBox.Extents.x;
		bool isIntervenedX = (minX <= position.x) && (position.x <= maxX);

		float minZ = node->boundingBox.Center.z - node->boundingBox.Extents.z;
		float maxZ = node->boundingBox.Center.z + node->boundingBox.Extents.z;
		bool isIntervenedZ = (minZ <= position.z)  && (position.z <= maxZ);
		 
		if (isIntervenedX && isIntervenedZ)
		{  
			// ���� ���Ѵٸ� �ش� ID�� ä���.
			assert(!(pIDs[QUAD - 1] != -1)); // ���� �������� ä���� �ִٸ� �����̴�.
			for (int x = 0; x < QUAD; ++x)
			{
				if (pIDs[x] == -1)
				{
					pIDs[x] = node->id; 
					break;
				}
			} 
		}
	}
	else
	{
		RecursiveCalculateIDs(node->children[0], position, pIDs);
		RecursiveCalculateIDs(node->children[1], position, pIDs);
		RecursiveCalculateIDs(node->children[2], position, pIDs);
		RecursiveCalculateIDs(node->children[3], position, pIDs);
	}
}

void QuadtreeTerrain::RecursiveCreateTerrain(QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList,
	int xStart, int zStart, int nBlockWidth, int nBlockLength,
	 HeightMapImage * pContext)
{
	assert(!(m_widthMin < 3));
	assert(!(m_lengthMin < 3));
	assert(!(m_lengthMin != m_widthMin));

	if (nBlockWidth == m_widthMin || nBlockLength == m_lengthMin) // ������ ���� �����..
	{ 
		// ������ ���� ���� ���̵� �����Ѵ�.
		node->id = gTreePieceCount++;
		m_ReafNodeCount += 1;

		// ���� �׽�Ʈ�� �ٿ���ڽ��� centerY = 128, externY = 256 ���� ���� 
		XMFLOAT3 center{
			float(xStart) * m_xmf3Scale.x + float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x ,
			0.0f,
			float(zStart) * m_xmf3Scale.z + float(nBlockLength - 1) / 2.0f * m_xmf3Scale.z };

		XMFLOAT3 extents{
				float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x,
				1000.f,
				float(nBlockLength - 1) / 2.0f* m_xmf3Scale.z };

		node->boundingBox = BoundingBox( center, extents); 
		node->terrainMesh = new TerrainMesh(this, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);
	}
	else
	{ 
		int cxQuadsPerBlock = nBlockWidth - 1;
		int czQuadsPerBlock = nBlockLength - 1;

		int Next_BlockWidth = cxQuadsPerBlock / 2 + 1;
		int Next_BlockLength = czQuadsPerBlock / 2 + 1;

		int index = 0;
		 
		for (int z = 0; z < 2; z++)
		{
			for (int x = 0; x < 2; x++)
			{
				int New_xStart = xStart + x * (Next_BlockWidth - 1);
				int New_zStart = zStart + z * (Next_BlockLength - 1);

				XMFLOAT3 center{ 
					float(xStart) * m_xmf3Scale.x + float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x ,
					0.0f,
					float(zStart) * m_xmf3Scale.z + float(nBlockLength - 1) / 2.0f * m_xmf3Scale.z };
				
				XMFLOAT3 extents{
						float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x,
						1000.f,
						float(nBlockLength - 1) / 2.0f* m_xmf3Scale.z  };

				node->boundingBox = BoundingBox(center, extents);

				node->children[index] = new QUAD_TREE_NODE(); 
				RecursiveCreateTerrain(node->children[index], pd3dDevice, pd3dCommandList, New_xStart, New_zStart, Next_BlockWidth, Next_BlockLength, pContext);
				index += 1;
			}
		} 
	}
}

UINT QuadtreeTerrain::CalculateVertex(UINT widht, UINT length)
{
	return (widht* length);
}

QuadtreeTerrain::QuadtreeTerrain(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, HeightMapImage * pContext)
	: GameObject("QuadTreeTerrain")
{

	m_widthTotal = nWidth;
	m_lengthTotal = nLength;
	m_xmf3Scale = xmf3Scale;
	m_xmf4Color = xmf4Color;

	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	// ���� Ʈ���� �θ� ��带 ����ϴ�.
	m_pRootNode = new QUAD_TREE_NODE;
	 
	// ��������� ������ ���ϰ�, �ٿ���ڽ��� �ͷ��� ������ �����Ѵ�.
	// ��������X
	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);
	m_pReafNodes = new QUAD_TREE_NODE*[m_ReafNodeCount]; //������带 ����ų ������ �迭�� ����
	m_RenderingIndices = new int[m_ReafNodeCount];
	RecursiveInitReafNodes(m_pRootNode);
	// ��������X

	// ����Լ��� ��� �ͷ��� ���� �ε� �Ϸ���...
	StaticObjectStorage::GetInstance(this)->CreateInfo(pd3dDevice, pd3dCommandList, this);
}

QuadtreeTerrain::~QuadtreeTerrain()
{
	if (m_pRootNode)
	{
		RecursiveReleaseObjects(m_pRootNode);
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
}

void QuadtreeTerrain::Update(float fElapsedTime)
{
}

void QuadtreeTerrain::LastUpdate(float fElapsedTime)
{
	// �׷����ϴ� ������ �ε��� �ʱ�ȭ
	for (int i = 0; i < m_ReafNodeCount; ++i) m_RenderingIndices[i] = -1;

	int RenderingIndexCount = 0;
	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		if (m_pReafNodes[i]->isRendering)
		{
			m_RenderingIndices[RenderingIndexCount++] = i;
		}
	}
}

int const * QuadtreeTerrain::GetIDs(const XMFLOAT3 & position) const
{
	int* pIDs = new int[QUAD];
	for (int x = 0; x < QUAD; ++x)
	{
		pIDs[x] = -1; // -1�� ����. -1�̶�� �������� �ʴ� ��.
	}
	
	RecursiveCalculateIDs(m_pRootNode, position, pIDs);

	return pIDs;
}

void QuadtreeTerrain::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	RecursiveRender(m_pRootNode, pd3dCommandList);
	RenderTerrainObjects(pd3dCommandList);
}

void QuadtreeTerrain::Render(int index, ID3D12GraphicsCommandList * pd3dCommandList)
{
	// ������
	extern MeshRenderer gMeshRenderer;

	if (index < 0 || index >= m_ReafNodeCount) return;
	//pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
	gMeshRenderer.Render(pd3dCommandList, m_pReafNodes[index]->terrainMesh);
	//StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, m_pReafNodes[index]->id);
}
