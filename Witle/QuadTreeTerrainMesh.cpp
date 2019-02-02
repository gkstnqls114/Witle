#include "stdafx.h"
#include "d3dUtil.h"
#include "TerrainMesh.h" 
#include "MeshRenderer.h"
#include "HeightMapImage.h"
#include "MyFrustum.h"
#include "QuadTreeTerrainMesh.h"

UINT QuadTreeTerrainMesh::CalculateTriangles(UINT widthPixel, UINT lengthPixel)
{
	assert(!(widthPixel != lengthPixel));
	
	return (widthPixel - 1) * (lengthPixel -1) * 2;
}

void QuadTreeTerrainMesh::RecursiveCreateTerrain(QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, 
	int xStart, int zStart, int nBlockWidth, int nBlockLength,
	int internal, HeightMapImage * pContext)
{
	assert(!(m_widthMin < 3));
	assert(!(m_lengthMin < 3));
	assert(!(m_lengthMin != m_widthMin));

	if (nBlockWidth == m_widthMin || nBlockLength == m_lengthMin) // ������ ���� ���
	{
		// �ͷ����� �����Ѵ�.
		static int numLeaf = 0;
		node->numLeaf = numLeaf;
		node->numInternal = internal;
		// ���� �׽�Ʈ�� centerY = 128, externY = 256 ���� ����

		node->boundingBox = BoundingBox(XMFLOAT3{ 
			float(xStart) * m_xmf3Scale.x + float(nBlockWidth) / 2.0f * m_xmf3Scale.x , 
			0.0f,
			float(zStart) * m_xmf3Scale.z + float(nBlockLength) / 2.0f * m_xmf3Scale.z },
			XMFLOAT3{ float(nBlockWidth) / 2.0f * m_xmf3Scale.x, 1000.f, float(nBlockLength) / 2.0f* m_xmf3Scale.z });

		node->terrainMesh = new TerrainMesh(m_pOwner, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);

		printf("������ %d ��° ����... ������ ����ũ��: %d X %d (%d, %d ���� ����)\n", numLeaf++, nBlockWidth, nBlockLength, xStart, zStart);
	}
	else
	{
		static int numInternal = 0;

		int cxQuadsPerBlock = nBlockWidth - 1;
		int czQuadsPerBlock = nBlockLength - 1;

		int Next_BlockWidth = cxQuadsPerBlock / 2 + 1;
		int Next_BlockLength = czQuadsPerBlock / 2 + 1;

		int index = 0;

		node->numInternal = numInternal;

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
				RecursiveCreateTerrain(node->children[index], pd3dDevice, pd3dCommandList, New_xStart, New_zStart, Next_BlockWidth, Next_BlockLength, numInternal, pContext);
				index += 1;
			}
		}
		numInternal += 1;
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

	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, 0, pContext);
}

QuadTreeTerrainMesh::~QuadTreeTerrainMesh()
{

}

void QuadTreeTerrainMesh::ReleaseUploadBuffers()
{
	//// ��� ������ Ʈ�� �Ʒ��� ������ �� �Ʒ� ��带 ���� �����ϴ�.
	//for (int i = 0; i < 4; i++)
	//{
	//	if (node->children[i] != 0)
	//	{
	//		ReleaseNode(node->children[i]);
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
	//	if (node->children[i])
	//	{
	//		delete node->children[i];
	//		node->children[i] = 0;
	//	}
	//}
}

void QuadTreeTerrainMesh::TESTRender(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList)
{
	// ������
	extern MeshRenderer gMeshRenderer;

	if (node->terrainMesh)
	{
		gMeshRenderer.Render(pd3dCommandList, node->terrainMesh);
	}
	else
	{
		if (node->children[0]->isRendering)
		{
			printf("%d - %d Render\n", node->children[0]->numInternal, node->children[0]->numLeaf);
			TESTRender(node->children[0], pd3dCommandList);
		}
		if (node->children[1]->isRendering)
		{
			printf("%d - %d Render\n", node->children[1]->numInternal, node->children[1]->numLeaf);
			TESTRender(node->children[1], pd3dCommandList);
		}
		if (node->children[2]->isRendering)
		{
			printf("%d - %d Render\n", node->children[2]->numInternal, node->children[2]->numLeaf);
			TESTRender(node->children[2], pd3dCommandList);
		}
		if (node->children[3]->isRendering)
		{
			printf("%d - %d Render\n", node->children[3]->numInternal, node->children[3]->numLeaf);
			TESTRender(node->children[3], pd3dCommandList);
		}
	}

	printf("\n");
}
