#include "stdafx.h"
#include "d3dUtil.h"

#include "TextureStorage.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "UI3DImage.h"

#define RECTANGLE_VERTEX_COUNT 6

void UI3DImage::ReleaseObjects()
{ 

}

void UI3DImage::ReleaseUploadBuffers()
{

}

UI3DImage::UI3DImage(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, RECT rect, const std::string& texture_name)
	:UI3D(pOwner)
{   
	m_ComponenetID = SHAPE_TYPE_ID::RECTANGLE_SHAPE;

	m_pTexture = TextureStorage::GetInstance()->GetTexture(texture_name);

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = RECTANGLE_VERTEX_COUNT;
	m_nStride = sizeof(UIVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	XMFLOAT3 pxmf3Positions[RECTANGLE_VERTEX_COUNT];
	// 상단 삼각형
	pxmf3Positions[0] = XMFLOAT3(rect.left, rect.top, 0.5f);
	pxmf3Positions[1] = XMFLOAT3(rect.right, rect.top, 0.5f);
	pxmf3Positions[2] = XMFLOAT3(rect.left, rect.bottom, 0.5f);
	// 하단 삼각형
	pxmf3Positions[3] = XMFLOAT3(rect.left, rect.bottom, 0.5f);
	pxmf3Positions[4] = XMFLOAT3(rect.right, rect.top, 0.5f);
	pxmf3Positions[5] = XMFLOAT3(rect.right, rect.bottom, 0.5f);

	XMFLOAT2 pxmf2UVs[RECTANGLE_VERTEX_COUNT];
	pxmf2UVs[0] = XMFLOAT2{ 0.f, 0.f };
	pxmf2UVs[1] = XMFLOAT2{ 1.f, 0.f };
	pxmf2UVs[2] = XMFLOAT2{ 0.f, 1.f };
	pxmf2UVs[3] = XMFLOAT2{ 0.f, 1.f };
	pxmf2UVs[4] = XMFLOAT2{ 1.f, 0.f };
	pxmf2UVs[5] = XMFLOAT2{ 1.f, 1.f };

	UIVertex pVertices[RECTANGLE_VERTEX_COUNT];
	for (int x = 0; x < RECTANGLE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = UIVertex(pxmf3Positions[x], pxmf2UVs[x]);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
}

UI3DImage::UI3DImage(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, POINT center, float width, float height, const std::string& texture_name)
	: UI3D(pOwner)
{
	m_ComponenetID = SHAPE_TYPE_ID::RECTANGLE_SHAPE;
	 
	m_pTexture = TextureStorage::GetInstance()->GetTexture(texture_name);

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = RECTANGLE_VERTEX_COUNT;
	m_nStride = sizeof(UIVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	RECT rect{ center.x - width / 2.f, center.y - height / 2.f, center.x + width / 2.f, center.y + height / 2.f };

	XMFLOAT3 pxmf3Positions[RECTANGLE_VERTEX_COUNT];
	// 상단 삼각형
	pxmf3Positions[0] = XMFLOAT3(rect.left, rect.top, 0.5f);
	pxmf3Positions[1] = XMFLOAT3(rect.right, rect.top, 0.5f);
	pxmf3Positions[2] = XMFLOAT3(rect.left, rect.bottom, 0.5f);
	// 하단 삼각형
	pxmf3Positions[3] = XMFLOAT3(rect.left, rect.bottom, 0.5f);
	pxmf3Positions[4] = XMFLOAT3(rect.right, rect.top, 0.5f);
	pxmf3Positions[5] = XMFLOAT3(rect.right, rect.bottom, 0.5f);

	XMFLOAT2 pxmf2UVs[RECTANGLE_VERTEX_COUNT];
	pxmf2UVs[0] = XMFLOAT2{ 0.f, 0.f };
	pxmf2UVs[1] = XMFLOAT2{ 1.f, 0.f };
	pxmf2UVs[2] = XMFLOAT2{ 0.f, 1.f };
	pxmf2UVs[3] = XMFLOAT2{ 0.f, 1.f };
	pxmf2UVs[4] = XMFLOAT2{ 1.f, 0.f };
	pxmf2UVs[5] = XMFLOAT2{ 1.f, 1.f };

	UIVertex pVertices[RECTANGLE_VERTEX_COUNT];
	for (int x = 0; x < RECTANGLE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = UIVertex(pxmf3Positions[x], pxmf2UVs[x]);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
}

UI3DImage::~UI3DImage()
{

}
 
void UI3DImage::Update(float fElapsedTime)
{

}

void UI3DImage::Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4& world)
{   
	m_pTexture->UpdateShaderVariables(pd3dCommandList);

	// 월드행렬 설정
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&world)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
}
