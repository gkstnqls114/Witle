#include "stdafx.h"
#include "d3dUtil.h"

// GameBase////////////////////////
#include "GameScreen.h"
#include "GameTimer.h"
// GameBase////////////////////////

// Manager / Stroage ////////////////////////
#include "TextureStorage.h"
#include "ShaderManager.h"
// Manager / Stroage ////////////////////////

#include "Texture.h" 
#include "MyDescriptorHeap.h"
#include "Shader.h"

#include "MyRectangle.h"

#define RECTANGLE_VERTEX_COUNT 6
 
void MyRectangle::ReleaseObjects()
{
	Shape::ReleaseObjects();
}

void MyRectangle::ReleaseUploadBuffers()
{
	Shape::ReleaseUploadBuffers(); 
}

MyRectangle::MyRectangle(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, RECT rect, const char * filepath)
	:Shape(pOwner) 
{
	m_ComponenetID = SHAPE_TYPE_ID::RECTANGLE_SHAPE;

	if (filepath)
	{
		m_pTexture = TextureStorage::GetInstance()->GetTexture(filepath);
	}
	
	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = RECTANGLE_VERTEX_COUNT;
	m_nStride = sizeof(RectVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	XMFLOAT3 pxmf3Positions[RECTANGLE_VERTEX_COUNT];
	// ��� �ﰢ��
	pxmf3Positions[0] = XMFLOAT3( rect.left,   rect.top, 0.5f);
	pxmf3Positions[1] = XMFLOAT3( rect.right,  rect.top, 0.5f);
	pxmf3Positions[2] = XMFLOAT3( rect.left,   rect.bottom, 0.5f);
	// �ϴ� �ﰢ��
	pxmf3Positions[3] = XMFLOAT3( rect.left,   rect.bottom, 0.5f);
	pxmf3Positions[4] = XMFLOAT3( rect.right,  rect.top, 0.5f);
	pxmf3Positions[5] = XMFLOAT3( rect.right,  rect.bottom, 0.5f);

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

MyRectangle::MyRectangle(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, POINT center, float width, float height, const char * filepath)
	: Shape(pOwner)
{
	m_ComponenetID = SHAPE_TYPE_ID::RECTANGLE_SHAPE;

	if (filepath)
	{
		m_pTexture = TextureStorage::GetInstance()->GetTexture(filepath);
	}

	m_nVertexBufferViews = 1;
	m_pVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_vertexCount = RECTANGLE_VERTEX_COUNT;
	m_nStride = sizeof(RectVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	RECT rect{ center.x - width / 2.f, center.y - height/2.f, center.x + width/2.f, center.y + height / 2.f };

	XMFLOAT3 pxmf3Positions[RECTANGLE_VERTEX_COUNT];
	// ��� �ﰢ��
	pxmf3Positions[0] = XMFLOAT3(rect.left, rect.top, 0.5f);
	pxmf3Positions[1] = XMFLOAT3(rect.right, rect.top, 0.5f);
	pxmf3Positions[2] = XMFLOAT3(rect.left, rect.bottom, 0.5f);
	// �ϴ� �ﰢ��
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

	RectVertex pVertices[RECTANGLE_VERTEX_COUNT];
	for (int x = 0; x < RECTANGLE_VERTEX_COUNT; ++x)
	{
		pVertices[x] = RectVertex(pxmf3Positions[x], XMFLOAT4{ 1.f, 1.f, 1.f, 1.f }, pxmf2UVs[x]);
	}

	m_pPositionBuffer = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_vertexCount, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pPositionUploadBuffer);

	m_pVertexBufferViews[0].BufferLocation = m_pPositionBuffer->GetGPUVirtualAddress();
	m_pVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pVertexBufferViews[0].SizeInBytes = m_nStride * m_vertexCount;
} 

MyRectangle::~MyRectangle()
{
}
 
void MyRectangle::Render(ID3D12GraphicsCommandList * pd3dCommandList, const Shader * shader)
{
	pd3dCommandList->SetPipelineState(shader->GetPSO());
	  
	m_pTexture->UpdateShaderVariables(pd3dCommandList);
	 
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);
	
	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
}

void MyRectangle::Render(ID3D12GraphicsCommandList * pd3dCommandList, XMFLOAT2 pos, float time)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_PICKINGPOINT, false);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &pos, 0);	 
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_TIME, 1, &time, 0);

	if (m_pTexture) m_pTexture->UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
}

void MyRectangle::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	if (m_pTexture) m_pTexture->UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { m_pVertexBufferViews[0] };
	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);

	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
}

//
//void MyRectangle::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT2 & pos, float Time)
//{
//	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_PICKINGPOINT);
//	if (m_pHeap) m_pHeap->UpdateShaderVariable(pd3dCommandList);
//	if (m_pTexture) m_pTexture->UpdateShaderVariables(pd3dCommandList);
//	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &pos, 0);
//	 
//	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_TIME, 1, &Time, 0);
//
//	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
//
//	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { m_pVertexBufferViews[0] };
//	pd3dCommandList->IASetVertexBuffers(m_nSlot, _countof(pVertexBufferViews), pVertexBufferViews);
//
//	pd3dCommandList->DrawInstanced(m_vertexCount, 1, m_nOffset, 0);
//}
 