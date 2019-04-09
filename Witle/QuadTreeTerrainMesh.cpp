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

// 처음 아이디는 0으로 시작한다.
// 리프 노드만 아이디를 설정한다.
// 따라서 아이디는 0부터 시작한다.
int QuadTreeTerrainMesh::gTreePieceCount{ 0 }; 

UINT QuadTreeTerrainMesh::CalculateTriangles(UINT widthPixel, UINT lengthPixel)
{
	assert(!(widthPixel != lengthPixel));
	
	return (widthPixel - 1) * (lengthPixel -1) * 2;
}
  

void QuadTreeTerrainMesh::RenderTerrainObjects(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 그려야하는 렌더링 인덱스 초기화
	for (int i = 0; i < m_ReafNodeCount; ++i) m_RenderingIndices[i] = -1;

	int RenderingIndexCount = 0;
	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		if (m_pReafNodes[i]->isRendering)
		{
			m_RenderingIndices[RenderingIndexCount++] = i;
		}
	}
	StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, m_RenderingIndices, m_ReafNodeCount);
}

void QuadTreeTerrainMesh::RecursiveRender(const QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 렌더링
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

void QuadTreeTerrainMesh::RecursiveInitReafNodes(QUAD_TREE_NODE * node)
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

void QuadTreeTerrainMesh::RecursiveReleaseUploadBuffers(QUAD_TREE_NODE * node)
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

void QuadTreeTerrainMesh::RecursiveReleaseObjects(QUAD_TREE_NODE * node)
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

void QuadTreeTerrainMesh::RecursiveCalculateIDs(QUAD_TREE_NODE * node, const XMFLOAT3 position, int* pIDs) const
{
	if (node->terrainMesh)
	{
		assert(!(node->id == -1));
		// 포지션이 해당 메쉬에 맞는지 확인한다. 
		// x, z 사이에 있는지 검사한다.
		float minX = node->boundingBox.Center.x - node->boundingBox.Extents.x;
		float maxX = node->boundingBox.Center.x + node->boundingBox.Extents.x;
		bool isIntervenedX = (minX <= position.x) && (position.x <= maxX);

		float minZ = node->boundingBox.Center.z - node->boundingBox.Extents.z;
		float maxZ = node->boundingBox.Center.z + node->boundingBox.Extents.z;
		bool isIntervenedZ = (minZ <= position.z)  && (position.z <= maxZ);
		 
		if (isIntervenedX && isIntervenedZ)
		{  
			// 만약 속한다면 해당 ID를 채운다.
			assert(!(pIDs[QUAD - 1] != -1)); // 만약 마지막이 채워져 있다면 오류이다.
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

void QuadTreeTerrainMesh::RecursiveCreateTerrain(QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList,
	int xStart, int zStart, int nBlockWidth, int nBlockLength,
	 HeightMapImage * pContext)
{
	assert(!(m_widthMin < 3));
	assert(!(m_lengthMin < 3));
	assert(!(m_lengthMin != m_widthMin));

	if (nBlockWidth == m_widthMin || nBlockLength == m_lengthMin) // 마지막 리프 노드라면..
	{ 
		// 마지막 리프 노드는 아이디를 설정한다.
		node->id = gTreePieceCount++;
		m_ReafNodeCount += 1;

		// 현재 테스트로 바운딩박스의 centerY = 128, externY = 256 으로 설정 
		XMFLOAT3 center{
			float(xStart) * m_xmf3Scale.x + float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x ,
			0.0f,
			float(zStart) * m_xmf3Scale.z + float(nBlockLength - 1) / 2.0f * m_xmf3Scale.z };

		XMFLOAT3 extents{
				float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x,
				1000.f,
				float(nBlockLength - 1) / 2.0f* m_xmf3Scale.z };

		node->boundingBox = BoundingBox( center, extents); 
		node->terrainMesh = new TerrainMesh(m_pOwner, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);
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

UINT QuadTreeTerrainMesh::CalculateVertex(UINT widht, UINT length)
{
	return (widht* length);
}

QuadTreeTerrainMesh::QuadTreeTerrainMesh(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, HeightMapImage * pContext)
	: ComponentBase(pOwner)
{
	m_FamilyID.InitTEST();
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
	 
	// 리프노드의 개수를 구하고, 바운딩박스및 터레인 조각을 생성한다.
	// 순서변경X
	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);
	m_pReafNodes = new QUAD_TREE_NODE*[m_ReafNodeCount]; //리프노드를 가리킬 포인터 배열을 생성
	m_RenderingIndices = new int[m_ReafNodeCount];
	RecursiveInitReafNodes(m_pRootNode);
	// 순서변경X

	// 재귀함수로 모든 터레인 조각 로드 완료후...
	StaticObjectStorage::GetInstance(this)->CreateInfo(pd3dDevice, pd3dCommandList, this);
}

QuadTreeTerrainMesh::~QuadTreeTerrainMesh()
{
	if (m_pRootNode)
	{
		RecursiveReleaseObjects(m_pRootNode);
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
}

void QuadTreeTerrainMesh::ReleaseUploadBuffers()
{
	RecursiveReleaseUploadBuffers(m_pRootNode); 
}

int const * QuadTreeTerrainMesh::GetIDs(const XMFLOAT3 & position) const
{
	int* pIDs = new int[QUAD];
	for (int x = 0; x < QUAD; ++x)
	{
		pIDs[x] = -1; // -1로 리셋. -1이라면 존재하지 않는 것.
	}
	
	RecursiveCalculateIDs(m_pRootNode, position, pIDs);

	return pIDs;
}

void QuadTreeTerrainMesh::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	RecursiveRender(m_pRootNode, pd3dCommandList);
	RenderTerrainObjects(pd3dCommandList);
}

void QuadTreeTerrainMesh::Render(int index, ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 렌더링
	extern MeshRenderer gMeshRenderer;

	if (index < 0 || index >= m_ReafNodeCount) return;
	//pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
	gMeshRenderer.Render(pd3dCommandList, m_pReafNodes[index]->terrainMesh);
	//StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, m_pReafNodes[index]->id);
}
