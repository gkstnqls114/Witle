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

void QuadTreeTerrainMesh::RecursiveCreateTerrain(QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, 
	int xStart, int zStart, int nWidth, int nLength, int nBlockWidth, int nBlockLength,
	HeightMapImage * pContext)
{
	//assert(!((nWidth - 1) / 2 == 0));
	//assert(!((nLength - 1) / 2 == 0));
	//assert(!(nWidth < 3));
	//assert(!(nLength < 3));

	if (nBlockWidth < m_widthMin || nBlockLength < m_lengthMin) // ������ ���� ���
	{
		// �ͷ����� �����Ѵ�.
		node->terrainMesh = new TerrainMesh(m_pOwner, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);
	}
	else
	{
		int cxQuadsPerBlock = nBlockWidth - 1;
		int czQuadsPerBlock = nBlockLength - 1;

		long cxBlocks = (nWidth - 1) / cxQuadsPerBlock;
		long czBlocks = (nLength - 1) / czQuadsPerBlock;

		TerrainMesh *pterrainMesh = NULL;
		int index = 0;
		for (int z = 0, zStart = 0; z < czBlocks; z++)
		{
			for (int x = 0, xStart = 0; x < cxBlocks; x++)
			{
				xStart = x * (nBlockWidth - 1);
				zStart = z * (nBlockLength - 1);

				node->nodes[index++] = new QUAD_TREE_NODE();
				RecursiveCreateTerrain(node->nodes[x], pd3dDevice, pd3dCommandList, xStart, zStart, nWidth, nLength, cxQuadsPerBlock / 2 + 1, czQuadsPerBlock / 2 + 1, pContext);
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
	m_ComponenetID = MESH_TYPE_ID::QUADTREE_TERRAIN_MESH;

	m_widthTotal = nWidth;
	m_lengthTotal = nLength;
	m_xmf3Scale = xmf3Scale;
	m_xmf4Color = xmf4Color;

	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	// ���� Ʈ���� �θ� ��带 ����ϴ�.
	m_pRootNode = new QUAD_TREE_NODE;

	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, m_widthTotal /2 + 1, m_lengthTotal /2 + 1, pContext);
}

QuadTreeTerrainMesh::~QuadTreeTerrainMesh()
{

}

void QuadTreeTerrainMesh::ReleaseUploadBuffers()
{
	//// ��� ������ Ʈ�� �Ʒ��� ������ �� �Ʒ� ��带 ���� �����ϴ�.
	//for (int i = 0; i < 4; i++)
	//{
	//	if (node->nodes[i] != 0)
	//	{
	//		ReleaseNode(node->nodes[i]);
	//	}
	//}

	//// �� ����� ���ؽ� ���۸� �����Ѵ�.
	//if (node->vertexBuffer)
	//{
	//	node->vertexBuffer->Release();
	//	node->vertexBuffer = 0;
	//}

	//// �� ����� �ε��� ���۸� �����մϴ�.
	//if (node->indexBuffer)
	//{
	//	node->indexBuffer->Release();
	//	node->indexBuffer = ;
	//}

	//// �� ���� �ڽ� ��带 �����մϴ�.
	//for (int i = 0; i < 4; i++)
	//{
	//	if (node->nodes[i])
	//	{
	//		delete node->nodes[i];
	//		node->nodes[i] = 0;
	//	}
	//}
}
