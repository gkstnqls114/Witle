#include "stdafx.h"
#include "d3dUtil.h"  
#include "GameInput.h"
#include "GameScreen.h"
#include "UI2DImage.h"
#include "GraphicsRootSignatureMgr.h"
#include "ShaderManager.h" 
#include "GameObject.h"
#include "Texture.h"
#include "GameScene.h"

#include "WinScene.h"

ID3D12DescriptorHeap*		WinScene::m_pd3dCbvSrvDescriptorHeap;

D3D12_CPU_DESCRIPTOR_HANDLE	WinScene::m_d3dCbvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	WinScene::m_d3dCbvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	WinScene::m_d3dSrvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	WinScene::m_d3dSrvGPUDescriptorStartHandle;

D3D12_CPU_DESCRIPTOR_HANDLE	WinScene::m_d3dCbvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	WinScene::m_d3dCbvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	WinScene::m_d3dSrvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	WinScene::m_d3dSrvGPUDescriptorNextHandle;

WinScene::WinScene()
{

}

WinScene::~WinScene()
{

}
void WinScene::CreateCbvSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorNextHandle = m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorNextHandle = m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorNextHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorNextHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}

void WinScene::ReleaseHeaps()
{
	if (m_pd3dCbvSrvDescriptorHeap)
	{
		m_pd3dCbvSrvDescriptorHeap->Release();
		m_pd3dCbvSrvDescriptorHeap = nullptr;
	}
}

D3D12_GPU_DESCRIPTOR_HANDLE WinScene::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = m_d3dCbvGPUDescriptorNextHandle;
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		m_d3dCbvCPUDescriptorNextHandle.ptr = m_d3dCbvCPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, m_d3dCbvCPUDescriptorNextHandle);
		m_d3dCbvGPUDescriptorNextHandle.ptr = m_d3dCbvGPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}
	return(d3dCbvGPUDescriptorHandle);
}

D3D12_GPU_DESCRIPTOR_HANDLE WinScene::CreateShaderResourceViews(ID3D12Device * pd3dDevice, Texture * pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	assert(!(pTexture == nullptr));

	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorNextHandle;
	if (pTexture)
	{
		int nTextures = pTexture->GetTextures();
		int nTextureType = pTexture->GetTextureType();
		for (int i = 0; i < nTextures; i++)
		{
			ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
			D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
			D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorNextHandle);
			m_d3dSrvCPUDescriptorNextHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameter + i) : nRootParameter, m_d3dSrvGPUDescriptorNextHandle);
			m_d3dSrvGPUDescriptorNextHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
		}
	}
	return(d3dSrvGPUDescriptorHandle);
}

bool WinScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool WinScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;

		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case 'A':
			break;
		case 'W':
		case 'w':
			break;
		case 'S':
		case 's':
			break;
		default:
			break;
		}
		break;
	}

	return false;
}

void WinScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	BuildLightsAndMaterials(pd3dDevice, pd3dCommandList);

	// ī�޶� ����
	m_d3dViewport = D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<FLOAT>(GameScreen::GetWidth()) , static_cast<FLOAT>(GameScreen::GetHeight()), 0.0f, 1.0f };
	m_d3dScissorRect = D3D12_RECT{ 0, 0, static_cast<LONG>(GameScreen::GetWidth()) ,static_cast<LONG>(GameScreen::GetHeight()) };

	// ��ũ���� �� ����
	WinScene::CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 3);
 
	m_gameobject = new EmptyGameObject("back");
	m_Background = new UI2DImage(m_gameobject, ENUM_SCENE::SCENE_MAIN, pd3dDevice, pd3dCommandList, RECT{
		0, 0,
		static_cast<LONG>(GameScreen::GetWidth()), static_cast<LONG>(GameScreen::GetHeight()) },
		"Win"
		);

	WinScene::CreateShaderResourceViews(pd3dDevice, m_Background->GetpTexture(), ROOTPARAMETER_TEXTURE, true);
}

void WinScene::ReleaseObjects()
{
	ReleaseHeaps();
	if (m_gameobject)
	{
		m_gameobject->ReleaseObjects();
		delete m_gameobject;
		m_gameobject = nullptr;
	}
	if (m_Background)
	{ 
		m_Background->ReleaseObjects();
		delete m_Background;
		m_Background = nullptr;
	}
}

bool WinScene::ProcessInput(HWND hWnd, float ElapsedTime)
{
	return true;
}

void WinScene::UpdatePhysics(float ElapsedTime)
{
}

// ProcessInput�� ���� right, up, look, pos �� ���庯ȯ ��Ŀ� �����Ѵ�.
void WinScene::Update(float fElapsedTime)
{
}

void WinScene::LastUpdate(float fElapsedTime)
{

}

void WinScene::AnimateObjects(float fTimeElapsed)
{
}

void WinScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{
	pd3dCommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// Ŭ�� ȭ�� ����
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UISCREEN, false);

	GameScene::SetDescriptorHeap(pd3dCommandList);
	for (int i = 0; i < 100; ++i)
	{
		m_Background->Render(pd3dCommandList);
	}
}

void WinScene::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void WinScene::RenderForPlayerShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void WinScene::ReleaseUploadBuffers()
{
	if (m_gameobject) m_gameobject->ReleaseUploadBuffers();
	if (m_Background) m_Background->ReleaseUploadBuffers();
}

void WinScene::BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void WinScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

