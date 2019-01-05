#include "stdafx.h"
#include "CubeMesh.h"
#include "MeshRenderer.h"


void MeshRenderer::RenderCubeMesh(ID3D12GraphicsCommandList * pd3dCommandList, Mesh * mesh)
{
	//���� ���� ��� �ν��Ͻ� ���� �並 �Է�-���� �ܰ迡 �����Ѵ�.
	CubeMesh *cubeMesh = static_cast<CubeMesh *>(mesh);
	int m_nSlot = 0;
	int m_nOffset = 0;
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology{ D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { cubeMesh->GetVertexBufferView(0) };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->DrawInstanced(cubeMesh->GetVertexCount(), 0, m_nOffset, 0);
}

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::Render(ID3D12GraphicsCommandList * pCommandList, Mesh * mesh)
{ 
	switch (mesh->GetComponentID())
	{
	case MESH_TYPE_ID::CUBE_MESH:
		RenderCubeMesh(pCommandList, mesh);
		break;

	default:
		break;
	}
}
