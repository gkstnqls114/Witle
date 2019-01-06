#include "stdafx.h"
#include "CubeMesh.h"
#include "MeshRenderer.h"


void MeshRenderer::RenderCubeMesh(ID3D12GraphicsCommandList * pd3dCommandList, Mesh * mesh)
{
	//정점 버퍼 뷰와 인스턴싱 버퍼 뷰를 입력-조립 단계에 설정한다.
	CubeMesh *cubeMesh = static_cast<CubeMesh *>(mesh);
	UINT m_nSlot = 0;
	UINT m_nOffset = 0;
	UINT nInstances = 1;
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology{ D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { cubeMesh->GetVertexBufferView(0) };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	pd3dCommandList->IASetIndexBuffer(&cubeMesh->GetIndexBufferView());
	pd3dCommandList->DrawIndexedInstanced(cubeMesh->GetIndexCount(), nInstances, 0, 0, 0);
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
