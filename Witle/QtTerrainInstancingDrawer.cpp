#include "stdafx.h"
#include "d3dUtil.h"
#include "TerrainMesh.h" 
#include "ShaderManager.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "TextureStorage.h"
#include "StaticObjectStorage.h"
#include "HeightMapImage.h"
#include <unordered_set>
#include "MyFrustum.h"
#include "Collision.h"
#include "Terrain.h"
#include "MyDescriptorHeap.h"
#include "QtTerrainInstancingDrawer.h"

// ó�� ���̵�� 0���� �����Ѵ�.
// ���� ��常 ���̵� �����Ѵ�.
// ���� ���̵�� 0���� �����Ѵ�.
int QtTerrainInstancingDrawer::gTreePieceCount{ 0 }; 
 
void QtTerrainInstancingDrawer::ReleaseMembers()
{
	delete[] m_pReafNodes; 
}

void QtTerrainInstancingDrawer::ReleaseMemberUploadBuffers()
{ 
	RecursiveReleaseUploadBuffers(m_pRootNode);
}


void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjects_BOBox(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList)
{
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			StaticObjectStorage::GetInstance(this)->RenderBOBox(pd3dCommandList, node->id);
		}
		else
		{

			if (node->children[0]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[0], pd3dCommandList);
			if (node->children[1]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[1], pd3dCommandList);
			if (node->children[2]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[2], pd3dCommandList);
			if (node->children[3]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[3], pd3dCommandList);

		}
	}
}


void QtTerrainInstancingDrawer::RenderTerrainObjects(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "InstancingStandardShader", isGBuffers);
	
	RecursiveRenderTerrainObjects(m_pRootNode, pd3dCommandList, isGBuffers);
	  
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "InstancingLine", isGBuffers);
	RecursiveRenderTerrainObjects_BOBox(m_pRootNode, pd3dCommandList); 
}

void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjects(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{ 
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, node->id, isGBuffers);

		}
		else
		{

			if (node->children[0]->isRendering) RecursiveRenderTerrainObjects(node->children[0], pd3dCommandList, isGBuffers);
			if (node->children[1]->isRendering) RecursiveRenderTerrainObjects(node->children[1], pd3dCommandList, isGBuffers);
			if (node->children[2]->isRendering) RecursiveRenderTerrainObjects(node->children[2], pd3dCommandList, isGBuffers);
			if (node->children[3]->isRendering) RecursiveRenderTerrainObjects(node->children[3], pd3dCommandList, isGBuffers);

		} 
 	} 
}


void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjectsForShadow(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			StaticObjectStorage::GetInstance(this)->RenderForShadow(pd3dCommandList, node->id, isGBuffers);

		}
		else
		{ 
			if (node->children[0]->isRendering) RecursiveRenderTerrainObjects(node->children[0], pd3dCommandList, isGBuffers);
			if (node->children[1]->isRendering) RecursiveRenderTerrainObjects(node->children[1], pd3dCommandList, isGBuffers);
			if (node->children[2]->isRendering) RecursiveRenderTerrainObjects(node->children[2], pd3dCommandList, isGBuffers);
			if (node->children[3]->isRendering) RecursiveRenderTerrainObjects(node->children[3], pd3dCommandList, isGBuffers);
		}
	}
}

static std::unordered_set<int> set;

void QtTerrainInstancingDrawer::RecursiveRender(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	// ������
	extern MeshRenderer gMeshRenderer;

	if (node->terrainMesh)
	{
		set.insert(node->id);
		gMeshRenderer.Render(pd3dCommandList, node->terrainMesh); 
	}
	else
	{
		if (node->children[0]->isRendering) RecursiveRender(node->children[0], pd3dCommandList, isGBuffers);
		if (node->children[1]->isRendering) RecursiveRender(node->children[1], pd3dCommandList, isGBuffers);
		if (node->children[2]->isRendering) RecursiveRender(node->children[2], pd3dCommandList, isGBuffers);
		if (node->children[3]->isRendering) RecursiveRender(node->children[3], pd3dCommandList, isGBuffers);
	}
}

void QtTerrainInstancingDrawer::RecursiveInitReafNodes(quadtree::QUAD_TREE_NODE * node)
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

void QtTerrainInstancingDrawer::RecursiveReleaseUploadBuffers(quadtree::QUAD_TREE_NODE * node)
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

void QtTerrainInstancingDrawer::RecursiveReleaseObjects(quadtree::QUAD_TREE_NODE * node)
{
	if (node->terrainMesh)
	{
		node->terrainMesh->ReleaseObjects();
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

void QtTerrainInstancingDrawer::RecursiveCalculateIDs(quadtree::QUAD_TREE_NODE * node, const XMFLOAT3 position, int* pIDs) const
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

void QtTerrainInstancingDrawer::CalculateIDs(const XMFLOAT3 position, XMINT4& IDs) const
{
	int num = 0;

	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		quadtree::QUAD_TREE_NODE* node = m_pReafNodes[i];

		if (Collision::isIn(node->boundingBox, position))
		{
			if (num == 0) IDs.x = i;
			else if (num == 1) IDs.y = i;
			else if (num == 2) IDs.z = i;
			else if (num == 3) IDs.w = i;
			++num;
		} 
		 
	}
}

void QtTerrainInstancingDrawer::CalculateIndex(const XMFLOAT3 position, int * pIndices) const
{
	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		quadtree::QUAD_TREE_NODE* node = m_pReafNodes[i];
		// �������� �ش� �޽��� �´��� Ȯ���Ѵ�. 
		// x, z ���̿� �ִ��� �˻��Ѵ�.
		float minX = node->boundingBox.Center.x - node->boundingBox.Extents.x;
		float maxX = node->boundingBox.Center.x + node->boundingBox.Extents.x;
		bool isIntervenedX = (minX <= position.x) && (position.x <= maxX);

		float minZ = node->boundingBox.Center.z - node->boundingBox.Extents.z;
		float maxZ = node->boundingBox.Center.z + node->boundingBox.Extents.z;
		bool isIntervenedZ = (minZ <= position.z) && (position.z <= maxZ);

		if (isIntervenedX && isIntervenedZ)
		{
			// ���� ���Ѵٸ� �ش� ID�� ä���.
			assert(!(pIndices[QUAD - 1] != -1)); // ���� �������� ä���� �ִٸ� �����̴�.
			for (int x = 0; x < QUAD; ++x)
			{
				if (pIndices[x] == -1)
				{
					pIndices[x] = i;
					break;
				}
			}
		}
	} 
}

void QtTerrainInstancingDrawer::RecursiveCreateTerrain(quadtree::QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList,
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

				node->children[index] = new quadtree::QUAD_TREE_NODE(); 
				RecursiveCreateTerrain(node->children[index], pd3dDevice, pd3dCommandList, New_xStart, New_zStart, Next_BlockWidth, Next_BlockLength, pContext);
				index += 1;
			}
		} 
	}
}
 
QtTerrainInstancingDrawer::QtTerrainInstancingDrawer(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, HeightMapImage * pContext)
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
	m_pRootNode = new quadtree::QUAD_TREE_NODE;
	 
	// ��������� ������ ���ϰ�, �ٿ���ڽ��� �ͷ��� ������ �����Ѵ�.
	// ��������X
	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);
	m_pReafNodes = new quadtree::QUAD_TREE_NODE*[m_ReafNodeCount]; //������带 ����ų ������ �迭�� ����
	RecursiveInitReafNodes(m_pRootNode);
	// ��������X

	// ����Լ��� ��� �ͷ��� ���� �ε� �Ϸ���...
	StaticObjectStorage::GetInstance(this)->CreateInfo(pd3dDevice, pd3dCommandList, this);
	 
}

QtTerrainInstancingDrawer::~QtTerrainInstancingDrawer()
{
	if (m_pRootNode)
	{
		RecursiveReleaseObjects(m_pRootNode);
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
}

void QtTerrainInstancingDrawer::Update(float fElapsedTime)
{
}

void QtTerrainInstancingDrawer::LastUpdate(float fElapsedTime)
{
	// �׷����ϴ� ������ �ε��� �ʱ�ȭ

}

XMINT4 const  QtTerrainInstancingDrawer::GetIDs(const XMFLOAT3 & position) const
{
	XMINT4 IDs{ -1, -1, -1, -1 };

	CalculateIDs(position, IDs);

	return IDs;
}

int * const QtTerrainInstancingDrawer::GetIndex(const XMFLOAT3 & position) const
{
	int* pIndeics = new int[QUAD];
	for (int x = 0; x < QUAD; ++x)
	{
		pIndeics[x] = -1; // -1�� ����. -1�̶�� �������� �ʴ� ��.
	}

	CalculateIndex(position, pIndeics);

	return pIndeics;
}

void QtTerrainInstancingDrawer::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{ 
	RenderTerrainObjects(pd3dCommandList, isGBuffers); // ���� ������Ʈ ����

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);
	RecursiveRender(m_pRootNode, pd3dCommandList, isGBuffers); // ���� ����	 
}

void QtTerrainInstancingDrawer::RenderTerrainForShadow(ID3D12GraphicsCommandList * pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap)
{ 
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN_FORSHADOW, false);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0);
	pd3dCommandList->SetDescriptorHeaps(1, &pHeap);
	pTerrain->UpdateShaderVariables(pd3dCommandList);

	RecursiveRender(m_pRootNode, pd3dCommandList, false); // ���� ����	 
}

void QtTerrainInstancingDrawer::RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_INSTACINGSTANDARDFORSHADOW, false);

	// ������ �� ���� 
	RecursiveRenderTerrainObjectsForShadow(m_pRootNode, pd3dCommandList, false);
}

void QtTerrainInstancingDrawer::Render(ID3D12GraphicsCommandList * pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers)
{
	// ���� ����
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0); 
	
	pTerrain->UpdateShaderVariables(pd3dCommandList);

	RecursiveRender(m_pRootNode, pd3dCommandList, isGBuffers); // ���� ����	 

	// ���� ������Ʈ ����
	RenderTerrainObjects(pd3dCommandList, isGBuffers);

}

void QtTerrainInstancingDrawer::Render(int index, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	// ������
	extern MeshRenderer gMeshRenderer; 
	if (index < 0 || index >= m_ReafNodeCount) return;
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);
	gMeshRenderer.Render(pd3dCommandList, m_pReafNodes[index]->terrainMesh);
	StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, index, isGBuffers);
}

quadtree::QUAD_TREE_NODE * QtTerrainInstancingDrawer::GetReafNodeByID(int id)
{
	return nullptr;
}
