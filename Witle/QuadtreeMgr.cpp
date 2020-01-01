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
#include "QuadtreeMgr.h"

// 처음 아이디는 0으로 시작한다.
// 리프 노드만 아이디를 설정한다.
// 따라서 아이디는 0부터 시작한다.
int QuadtreeMgr::gTreePieceCount{ 0 };

UINT QuadtreeMgr::CalculateTriangles(UINT widthPixel, UINT lengthPixel)
{
	assert(!(widthPixel != lengthPixel));

	return (widthPixel - 1) * (lengthPixel - 1) * 2;
}
  
void QuadtreeMgr::RecursiveInitReafNodes(QUAD_TREE_NODE* node)
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

void QuadtreeMgr::RecursiveReleaseUploadBuffers(QUAD_TREE_NODE* node)
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

void QuadtreeMgr::RecursiveReleaseObjects(QUAD_TREE_NODE* node)
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

void QuadtreeMgr::RecursiveCalculateIDs(QUAD_TREE_NODE* node, const XMFLOAT3 position, int* pIDs) const
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
		bool isIntervenedZ = (minZ <= position.z) && (position.z <= maxZ);

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

void QuadtreeMgr::CalculateIDs(const XMFLOAT3 position, XMINT4& IDs) const
{
	int num = 0;

	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		QUAD_TREE_NODE* node = m_pReafNodes[i];

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

void QuadtreeMgr::CalculateIndex(const XMFLOAT3 position, int* pIndices) const
{
	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		QUAD_TREE_NODE* node = m_pReafNodes[i];
		// 포지션이 해당 메쉬에 맞는지 확인한다. 
		// x, z 사이에 있는지 검사한다.
		float minX = node->boundingBox.Center.x - node->boundingBox.Extents.x;
		float maxX = node->boundingBox.Center.x + node->boundingBox.Extents.x;
		bool isIntervenedX = (minX <= position.x) && (position.x <= maxX);

		float minZ = node->boundingBox.Center.z - node->boundingBox.Extents.z;
		float maxZ = node->boundingBox.Center.z + node->boundingBox.Extents.z;
		bool isIntervenedZ = (minZ <= position.z) && (position.z <= maxZ);

		if (isIntervenedX && isIntervenedZ)
		{
			// 만약 속한다면 해당 ID를 채운다.
			assert(!(pIndices[QUAD - 1] != -1)); // 만약 마지막이 채워져 있다면 오류이다.
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

void QuadtreeMgr::RecursiveCreateTerrain(QUAD_TREE_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
	int xStart, int zStart, int nBlockWidth, int nBlockLength,
	HeightMapImage* pContext)
{
	assert(!(m_widthMin < 3));
	assert(!(m_lengthMin < 3));
	assert(!(m_lengthMin != m_widthMin));

	if (nBlockWidth == m_widthMin || nBlockLength == m_lengthMin) // 마지막 리프 노드라면..
	{
		
	}
	else
	{

	}
}

UINT QuadtreeMgr::CalculateVertex(UINT widht, UINT length)
{
	return (widht * length);
}

QuadtreeMgr::QuadtreeMgr(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, HeightMapImage* pContext)
{
	m_widthTotal = nWidth;
	m_lengthTotal = nLength;
	m_xmf3Scale = xmf3Scale;
	m_xmf4Color = xmf4Color;

	HeightMapImage* pHeightMapImage = (HeightMapImage*)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	// 쿼드 트리의 부모 노드를 만듭니다.
	m_pRootNode = new QUAD_TREE_NODE;

	// 리프노드의 개수를 구하고, 바운딩박스및 터레인 조각을 생성한다.
	// 순서변경X
	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);
	m_pReafNodes = new QUAD_TREE_NODE * [m_ReafNodeCount]; //리프노드를 가리킬 포인터 배열을 생성
	RecursiveInitReafNodes(m_pRootNode);
	// 순서변경X
	 

}

QuadtreeMgr::~QuadtreeMgr()
{
	if (m_pRootNode)
	{
		RecursiveReleaseObjects(m_pRootNode);
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
}

void QuadtreeMgr::Update(float fElapsedTime)
{
}

void QuadtreeMgr::LastUpdate(float fElapsedTime)
{
	// 그려야하는 렌더링 인덱스 초기화

}

XMINT4 const  QuadtreeMgr::GetIDs(const XMFLOAT3& position) const
{
	XMINT4 IDs{ -1, -1, -1, -1 };

	CalculateIDs(position, IDs);

	return IDs;
}

int* const QuadtreeMgr::GetIndex(const XMFLOAT3& position) const
{
	int* pIndeics = new int[QUAD];
	for (int x = 0; x < QUAD; ++x)
	{
		pIndeics[x] = -1; // -1로 리셋. -1이라면 존재하지 않는 것.
	}

	CalculateIndex(position, pIndeics);

	return pIndeics;
}
 
QUAD_TREE_NODE* QuadtreeMgr::GetReafNodeByID(int id)
{
	return nullptr;
}
