#include "stdafx.h"
#include "d3dUtil.h"
#include "Texture.h"
#include "GameTimer.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "MyDescriptorHeap.h"
#include "EffectRect.h"

#define RECTANGLE_VERTEX_COUNT 6

void BroomEffectRect::ReleaseObjects()
{
	Shape::ReleaseObjects();
	if (m_pHeap)
	{
		m_pHeap->ReleaseObjects();
		delete m_pHeap;
	}
	if (m_pTexture)
	{
		m_pTexture->ReleaseObjects();
		delete m_pTexture;
	}
}

void BroomEffectRect::ReleaseUploadBuffers()
{
	Shape::ReleaseUploadBuffers();
	if (m_pTexture) m_pTexture->ReleaseUploadBuffers();
}

BroomEffectRect::BroomEffectRect(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	: Shape(pOwner)
{
	m_ComponenetID = SHAPE_TYPE_ID::BROOMEFFECT_SHAPE;

	m_pHeap = new MyDescriptorHeap();
	m_pHeap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Dust.dds", 0);
	m_pHeap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, true);

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = RECTANGLE_VERTEX_COUNT;
	m_nStride = sizeof(RectVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	POINT center{ 0, 0 };
	float width = 100.f, height = 100.f;
	RECT rect{ center.x - width / 2.f, center.y - height / 2.f, center.x + width / 2.f, center.y + height / 2.f };

	XMFLOAT3 pxmf3Positions[RECTANGLE_VERTEX_COUNT];
	// »ó´Ü »ï°¢Çü
	pxmf3Positions[0] = XMFLOAT3(rect.left, rect.top, 0.5f);
	pxmf3Positions[1] = XMFLOAT3(rect.right, rect.top, 0.5f);
	pxmf3Positions[2] = XMFLOAT3(rect.left, rect.bottom, 0.5f);
	// ÇÏ´Ü »ï°¢Çü
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

	XMFLOAT2 time[RECTANGLE_VERTEX_COUNT];
	for (int x = 0; x < RECTANGLE_VERTEX_COUNT; ++x)
	{
		time[x] = XMFLOAT2(1.F, 10.f);
	}

	XMFLOAT3 velocity[RECTANGLE_VERTEX_COUNT];
	for (int x = 0; x < RECTANGLE_VERTEX_COUNT; ++x)
	{
		velocity[x] = XMFLOAT3(0.f, 0.f, 0.f);
	}

	RectVertex pVertices[RECTANGLE_VERTEX_COUNT];
	for (int x = 0; x < RECTANGLE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = RectVertex
		(
			pxmf3Positions[x],
			pxmf2UVs[x],
			time[x],
			velocity[x]
		);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
}

BroomEffectRect::~BroomEffectRect()
{
}

void BroomEffectRect::Render(ID3D12GraphicsCommandList * pd3dCommandList, const Shader * shader)
{ 
	pd3dCommandList->SetPipelineState(shader->GetPSO()); 
	m_pHeap->UpdateShaderVariable(pd3dCommandList);
	m_pTexture->UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
}

void BroomEffectRect::Render(ID3D12GraphicsCommandList * pd3dCommandList, float fElapsedTime)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_PICKINGPOINT);
	m_pHeap->UpdateShaderVariable(pd3dCommandList);
	m_pTexture->UpdateShaderVariables(pd3dCommandList);
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &fElapsedTime, 0);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
}
