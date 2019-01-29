#include "stdafx.h"
#include "CubeMesh.h"
#include "TerrainMesh.h"
#include "MeshRenderer.h"

// CubeMesh�� �����Ѵ�.
void MeshRenderer::RenderCubeMesh(ID3D12GraphicsCommandList * pd3dCommandList, Mesh * mesh)
{
	//���� ���� ��� �ν��Ͻ� ���� �並 �Է�-���� �ܰ迡 �����Ѵ�.
	CubeMesh *cubeMesh = static_cast<CubeMesh *>(mesh); 

	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology{ D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { cubeMesh->GetVertexBufferView(0) };
	pd3dCommandList->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->IASetIndexBuffer(&cubeMesh->GetIndexBufferView());
	pd3dCommandList->DrawIndexedInstanced(cubeMesh->GetIndexCount(), 1, 0, 0, 0);
}

// TerrainMesh�� �����Ѵ�.
void MeshRenderer::RenderTerrainMesh(ID3D12GraphicsCommandList * pd3dCommandList, Mesh * mesh)
{
	TerrainMesh *terrainMesh = static_cast<TerrainMesh *>(mesh);

	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology{ D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP };
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { terrainMesh->GetVertexBufferView(0) };
	pd3dCommandList->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->IASetIndexBuffer(&terrainMesh->GetIndexBufferView());
	pd3dCommandList->DrawIndexedInstanced(terrainMesh->GetIndexCount(), 1, 0, 0, 0);
	
}

void MeshRenderer::RenderQuadTreeTerrainMesh(ID3D12GraphicsCommandList * pd3dCommandList, Mesh * mesh)
{

}

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
} 

void MeshRenderer::Render(ID3D12GraphicsCommandList * pCommandList, Mesh * mesh)
{ 
	switch (mesh->GetComponentID())
	{
	case MESH_TYPE_ID::CUBE_MESH:
		RenderCubeMesh(pCommandList, mesh);
		break;

	case MESH_TYPE_ID::TERRAIN_MESH:
		RenderTerrainMesh(pCommandList, mesh);
		break;

	case MESH_TYPE_ID::QUADTREE_TERRAIN_MESH:
		RenderQuadTreeTerrainMesh(pCommandList, mesh);
		break;
	
	default:
		break;
	}
}
