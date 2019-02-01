#include "stdafx.h"
#include "d3dUtil.h"
#include "TerrainMesh.h" 
#include "MeshRenderer.h"
#include "HeightMapImage.h"
#include "QuadTreeTerrainMesh.h"

UINT QuadTreeTerrainMesh::CalculateTriangles(UINT widthPixel, UINT lengthPixel)
{
	assert(!(widthPixel != lengthPixel));
	
	return (widthPixel - 1) * (lengthPixel -1) * 2;
}

void QuadTreeTerrainMesh::RecursiveCreateTerrain(QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, 
	int xStart, int zStart, int nWidth, int nLength, int nBlockWidth, int nBlockLength,
	HeightMapImage * pContext)
{
	//assert(!((nWidth - 1) / 2 == 0));
	//assert(!((nLength - 1) / 2 == 0));
	//assert(!(nWidth < 3));
	//assert(!(nLength < 3));

	if (nBlockWidth < m_widthMin || nBlockLength < m_lengthMin) // 마지막 리프 노드
	{
		// 터레인을 생성한다.
		static int num = 0;
		printf("index: %d (%d , %d , %d , %d , %d , %d)\n", num++, xStart, zStart, nWidth, nLength, nBlockWidth, nBlockLength);
		printf("생성된 지형 크기: %d , %d\n\n", m_widthMin, m_lengthMin);

		node->terrainMesh = new TerrainMesh(m_pOwner, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);
	}
	else
	{
		int cxQuadsPerBlock = nBlockWidth - 1;
		int czQuadsPerBlock = nBlockLength - 1;

		// long cxBlocks = (nWidth - 1) / cxQuadsPerBlock;
		//long cxBlocks = (nWidth - 1) / cxQuadsPerBlock;
		long cxBlocks = QUAD / 2;
		long czBlocks = QUAD / 2;

		TerrainMesh *pterrainMesh = NULL;
		static int first_num = 0;
		int index = 0;
		int Next_BlockWidth = cxQuadsPerBlock / 2 + 1;
		int Next_BlockLength = czQuadsPerBlock / 2 + 1;
		for (int z = 0; z < czBlocks; z++)
		{
			for (int x = 0; x < cxBlocks; x++)
			{
				int New_xStart = xStart + x * (nBlockWidth - 1);
				int New_zStart = zStart + z * (nBlockLength - 1);

				printf("\n%d .. %d \n ", first_num, index);
				node->nodes[index] = new QUAD_TREE_NODE(); 
				RecursiveCreateTerrain(node->nodes[index], pd3dDevice, pd3dCommandList, New_xStart, New_zStart, nWidth, nLength, Next_BlockWidth, Next_BlockLength, pContext);
				index += 1;
			}
		}

		first_num += 1;
	}
}

UINT QuadTreeTerrainMesh::CalculateVertex(UINT widht, UINT length)
{
	return (widht* length);
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

	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, m_widthTotal /2 + 1, m_lengthTotal /2 + 1, pContext);
}

QuadTreeTerrainMesh::~QuadTreeTerrainMesh()
{

}

void QuadTreeTerrainMesh::ReleaseUploadBuffers()
{
	//// 재귀 적으로 트리 아래로 내려와 맨 아래 노드를 먼저 놓습니다.
	//for (int i = 0; i < 4; i++)
	//{
	//	if (node->nodes[i] != 0)
	//	{
	//		ReleaseNode(node->nodes[i]);
	//	}
	//}

	//// 이 노드의 버텍스 버퍼를 해제한다.
	//if (node->vertexBuffer)
	//{
	//	node->vertexBuffer->Release();
	//	node->vertexBuffer = 0;
	//}

	//// 이 노드의 인덱스 버퍼를 해제합니다.
	//if (node->indexBuffer)
	//{
	//	node->indexBuffer->Release();
	//	node->indexBuffer = ;
	//}

	//// 네 개의 자식 노드를 해제합니다.
	//for (int i = 0; i < 4; i++)
	//{
	//	if (node->nodes[i])
	//	{
	//		delete node->nodes[i];
	//		node->nodes[i] = 0;
	//	}
	//}
}

void QuadTreeTerrainMesh::TESTRender(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList)
{
	// 렌더링
	extern MeshRenderer gMeshRenderer;

	if (node->terrainMesh)
	{
		gMeshRenderer.Render(pd3dCommandList, node->terrainMesh);
	}
	else
	{
		TESTRender(node->nodes[0], pd3dCommandList);
		TESTRender(node->nodes[1], pd3dCommandList);
		TESTRender(node->nodes[2], pd3dCommandList);
		TESTRender(node->nodes[3], pd3dCommandList);
	}

}
