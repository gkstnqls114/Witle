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
		// 포지션이 해당 메쉬에 맞는지 확인한다. 
		// x, z 사이에 있는지 검사한다.
		bool isIntervenedX = (node->boundingBox.Center.x - node->boundingBox.Extents.x <= position.x)
							  && (position.x <= node->boundingBox.Center.x + node->boundingBox.Extents.x);
		bool isIntervenedZ = (node->boundingBox.Center.z - node->boundingBox.Extents.z <= position.z)
							  && (position.z <= node->boundingBox.Center.z + node->boundingBox.Extents.z);

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
		// 마지막 노드는 아이디를 설정한다.
		node->id = gTreePieceCount++;

		// 현재 테스트로 바운딩박스의 centerY = 128, externY = 256 으로 설정
		node->boundingBox = BoundingBox(XMFLOAT3{ 
			float(xStart) * m_xmf3Scale.x + float(nBlockWidth) / 2.0f * m_xmf3Scale.x , 
			0.0f,
			float(zStart) * m_xmf3Scale.z + float(nBlockLength) / 2.0f * m_xmf3Scale.z },
			XMFLOAT3{ float(nBlockWidth) / 2.0f * m_xmf3Scale.x, 1000.f, float(nBlockLength) / 2.0f* m_xmf3Scale.z });

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

				node->boundingBox = BoundingBox(XMFLOAT3{
					float(xStart) * m_xmf3Scale.x + float(nBlockWidth) / 2.0f * m_xmf3Scale.x ,
					0.0f,
					float(zStart) * m_xmf3Scale.z + float(nBlockLength) / 2.0f * m_xmf3Scale.z },
					XMFLOAT3{ float(nBlockWidth) / 2.0f * m_xmf3Scale.x, 1000.f, float(nBlockLength) / 2.0f* m_xmf3Scale.z });

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

	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);

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

void QuadTreeTerrainMesh::Render(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList)
{
	// 렌더링
	extern MeshRenderer gMeshRenderer;

	if (node->terrainMesh)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
		gMeshRenderer.Render(pd3dCommandList, node->terrainMesh);
		StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, node->id);
	}
	else
	{
		for (int x = 0; x < QUAD; ++x)
		{ 
			if (node->children[x]->isRendering)
			{
				Render(node->children[x], pd3dCommandList);
			}
		} 
	}
}
