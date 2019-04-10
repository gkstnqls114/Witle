#include "stdafx.h"
#include "d3dUtil.h"
#include "ShaderManager.h"
#include "MyRectangle.h"

#define RECTANGLE_VERTEX_COUNT 6
 
MyRectangle::MyRectangle(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, RECT rect)
	:Shape(pOwner) 
{
	m_ComponenetID = SHAPE_TYPE_ID::RECTANGLE_SHAPE;

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = RECTANGLE_VERTEX_COUNT;
	m_nStride = sizeof(RectVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	XMFLOAT3 pxmf3Positions[RECTANGLE_VERTEX_COUNT];
	// »ó´Ü »ï°¢Çü
	pxmf3Positions[0] = XMFLOAT3(rect.left, rect.top, -1.f);
	pxmf3Positions[1] = XMFLOAT3(rect.right, rect.top, 1.f);
	pxmf3Positions[2] = XMFLOAT3(rect.left, rect.bottom, 1.f);
	// ÇÏ´Ü »ï°¢Çü
	pxmf3Positions[3] = XMFLOAT3(rect.left, rect.bottom, 1.f);
	pxmf3Positions[4] = XMFLOAT3(rect.right, rect.top, 1.f);
	pxmf3Positions[5] = XMFLOAT3(rect.right, rect.bottom, 1.f);

	XMFLOAT2 pxmf2UVs[RECTANGLE_VERTEX_COUNT]; 
	pxmf2UVs[0] = XMFLOAT2{ 0.f, 0.f };
	pxmf2UVs[1] = XMFLOAT2{ 1.f, 0.f };
	pxmf2UVs[2] = XMFLOAT2{ 0.f, 1.f };
	pxmf2UVs[3] = XMFLOAT2{ 0.f, 1.f };
	pxmf2UVs[4] = XMFLOAT2{ 1.f, 0.f };
	pxmf2UVs[5] = XMFLOAT2{ 1.f, 1.f };
	
	RectVertex pVertices[RECTANGLE_VERTEX_COUNT];
	for (int x = 0; x < RECTANGLE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = RectVertex(pxmf3Positions[x], XMFLOAT4{1.f, 1.f, 1.f, 1.f}, pxmf2UVs[x]);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
}

MyRectangle::MyRectangle(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float centerX, float centerY, float width, float height)
	: Shape(pOwner)
{
}

MyRectangle::~MyRectangle()
{
}

void MyRectangle::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "ScreenShader");

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { GetVertexBufferView(0) };
	pd3dCommandList->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);
	pd3dCommandList->DrawInstanced(m_vertexCount, 1, 0, 0);
}
