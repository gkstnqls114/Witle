#include "stdafx.h"
#include "d3dx12.h"
#include "d3dUtil.h"

//// GameBase ////////////////////////// 
#include "GameTimer.h"
#include "GameInput.h"
#include "GameScreen.h" 
//// GameBase ////////////////////////// 

//// Manager ////////////////////////// 
#include "MainCameraMgr.h"
#include "GraphicsRootSignatureMgr.h"
#include "ShaderManager.h"
#include "ModelStorage.h"
#include "TextureStorage.h"
#include "StaticObjectStorage.h"
#include "SceneMgr.h"
#include "LightManager.h"
#include "SkillStg.h"
#include "EffectMgr.h"
//// Manager ////////////////////////// 

//// Scene //////////////////////////  
#include "GameScene.h"
#include "SkillSelectScene.h"
//// Scene ////////////////////////// 
 
//// ETC ////////////////////////////
#include "HorizonBlurShader.h"
#include "VerticalBlurShader.h"
#include "DownScaleFirstPassShader.h"
#include "DownScaleSecondPassShader.h"
#include "BloomRevealShader.h"
#include "QuadtreeTerrain.h"
#include "ComputeShader.h"
#include "CameraObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Object.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
//// ETC ////////////////////////////
 
#include "GameFramework.h"

static const bool DefferedRendering = false;
static const bool isBloom = true;

void CGameFramework::Render()
{
	HRESULT hResult = m_CommandAllocator->Reset();
	hResult = m_CommandList->Reset(m_CommandAllocator.Get(), NULL);

	m_CommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	m_CommandList->SetComputeRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	if (DefferedRendering)
	{
		// ������ ���� �׸��ϴ�.
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		RenderOnRTs(&CGameFramework::RenderForShadow, 0, NULL, NULL, m_ShadowmapCPUHandle);
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
		// ������ ���� �׸��ϴ�.
		 
		// RenderOnGbuffer(); // G Buffer �� �������մϴ�.
		RenderOnRTs(&CGameFramework::RenderOnGbuffer, m_GBuffersCountForRenderTarget, m_GBuffersForRenderTarget, m_GBufferCPUHandleForRenderTarget, m_GBufferCPUHandleForDepth[0]);

		ToneMapping();

		DefferedRenderOnSwapchain(); 
	}
	else
	{
		if (isBloom)
		{
			// ������ ���� �׸��ϴ�.
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			RenderOnRTs(&CGameFramework::RenderForShadow, 0, NULL, NULL, m_ShadowmapCPUHandle);
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
			// ������ ���� �׸��ϴ�.

			// �÷��̾� ������ ���� �׸��ϴ�.
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_PlayerShadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			RenderOnRTs(&CGameFramework::RenderForPlayerShadow, 0, NULL, NULL, m_PlayerShadowmapCPUHandle);
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_PlayerShadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
			// ������ ���� �׸��ϴ�.
			 
			// ����ó���� ȭ���� GBuffer�� �׸��ϴ�. �׸��ϴ�.
			RenderOnRTs(&CGameFramework::RenderSwapChain, 1, m_GBuffersForRenderTarget, m_GBufferCPUHandleForRenderTarget, m_GBufferCPUHandleForDepth[0]);

			// gbuffer heap�� �����մϴ�.
			m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

			// �ֵ��� ���մϴ�.
			DownScale();

			// bloom �� ���� �ؽ��ĸ� ���մϴ�.
			Bloom();

			// Bloom �ؽ��ĸ� blur �մϴ�.
			Blur();

			// ������� �մϴ�.
			RenderOnRT(&CGameFramework::ToneMapping, m_RenderTargetBuffers[m_SwapChainBufferIndex], m_SwapChainCPUHandle[m_SwapChainBufferIndex], m_DepthStencilCPUHandle);
			 
		}
		else
		{
			// ������ ���� �׸��ϴ�.
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			RenderOnRTs(&CGameFramework::RenderForShadow, 0, NULL, NULL, m_ShadowmapCPUHandle);
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
			// ������ ���� �׸��ϴ�.

			// �÷��̾� ������ ���� �׸��ϴ�.
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_PlayerShadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			RenderOnRTs(&CGameFramework::RenderForPlayerShadow, 0, NULL, NULL, m_PlayerShadowmapCPUHandle);
			d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_PlayerShadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
			// ������ ���� �׸��ϴ�.

			// ����ó���� ȭ���� �׸��ϴ�.
			RenderOnRT(&CGameFramework::RenderSwapChain, m_RenderTargetBuffers[m_SwapChainBufferIndex], m_SwapChainCPUHandle[m_SwapChainBufferIndex], m_DepthStencilCPUHandle);
		}
	}

	//// SwapChain�� Render //////////////////////////


	hResult = m_CommandList->Close();
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	m_SwapChain->Present(0, 0);

	MoveToNextFrame();
}

void CGameFramework::Debug()
{
#if _DEBUG
	//// Map the data so we can read it on CPU.
	//float* mappedData1 = nullptr;
	//float* mappedData2 = nullptr;
	//ThrowIfFailed(m_ReadBackMiddleAvgLumBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData1)));
	//ThrowIfFailed(m_ReadBackAvgLumBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData2)));

	////for (int i = 0; i < NumMiddleAvgLum; ++i)
	////{
	////	std::cout << mappedData1[i] << " ";
	////}
	////std::cout << std::endl;

	//for (int i = 0; i < NumAvgLum; ++i)
	//{
	//	std::cout << mappedData2[i] << " ";
	//}
	//std::cout << std::endl;
	//std::cout << std::endl;

	//m_ReadBackMiddleAvgLumBuffer->Unmap(0, nullptr);
	//m_ReadBackAvgLumBuffer->Unmap(0, nullptr);
#endif // _DEBUG 
}

CGameFramework::CGameFramework()
{ 
}

CGameFramework::~CGameFramework()
{

}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;
	GameInput::SetHWND(hMainWnd);

	// ���� ���� ���� ���� ////////////
	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	CreateShadowmapView();
	CreateRWResourceViews();
	// ���� ���� ���� ���� ////////////

	BuildObjects();
	
	GameScreen::SetSwapChain(m_SwapChain.Get());

	return true;
}

void CGameFramework::OnDestroy()
{
	GameScreen::ChangeWindowScreen(GameScreen::GetWidth(), GameScreen::GetHeight());

	ReleaseObjects();

	::CloseHandle(m_hFenceEvent);

	if (m_RenderTargetBuffers)
	{
		for (int x = 0; x < m_SwapChainBuffersCount; ++x)
		{
			m_RenderTargetBuffers[x]->Release();
		}
	}

	if (m_DepthStencilBuffer)
	{
		m_DepthStencilBuffer->Release();
	}

	//if (m_pShadowMap)
	//{
	//	m_pShadowMap->Release();
	//}

	if (m_horizenShader)
	{
		delete m_horizenShader;
	}
	if (m_verticalShader)
	{
		delete m_verticalShader;
	}
}

void CGameFramework::RenderGBuffers()
{ 
	m_CommandList->OMSetRenderTargets(1, &m_SwapChainCPUHandle[m_SwapChainBufferIndex], TRUE, &m_DepthStencilCPUHandle);

	////�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	m_CommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	////���������� ���¸� �����Ѵ�.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);
	
	float anotherWidth = static_cast<float>(GameScreen::GetClientWidth()) / 4;
	float anotherHeight = static_cast<float>(GameScreen::GetClientHeight()) / 4;

	float width = static_cast<float>(GameScreen::GetClientWidth());
	float height = static_cast<float>(GameScreen::GetClientHeight());
	 
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

	// ���ҽ��� �ٲ۴�.. 
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_GBufferHeap->GetGPUDescriptorHandleForHeapStart();
	
	// ����� �����մϴ�.
	for (int i = 0; i < m_GBuffersCount; ++i)
	{
		D3D12_VIEWPORT	GBuffer_Viewport{ anotherWidth *  i, height - anotherHeight, anotherWidth , anotherHeight, 1.0f, 0.0f };
		D3D12_RECT		ScissorRect{ anotherWidth * i, height - anotherHeight, anotherWidth * (3 + i) , height + anotherHeight };

		m_CommandList->RSSetViewports(1, &GBuffer_Viewport);
		m_CommandList->RSSetScissorRects(1, &ScissorRect);

		m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, handle);
		handle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->DrawInstanced(6, 1, 0, 0);

	}
}


void CGameFramework::CreateRWResourceViews()
{ 
	// ���ҽ� ����
	// ���۵� �ƴϰ�, ���ҽ� ���°� ���� ���ٽǵ� ���� Ÿ�ٵ� �ƴ϶�� Ŭ�������� NULL

	m_RWHDRTex_1_16 = d3dUtil::CreateTexture2DResource
	(m_d3dDevice.Get(), m_CommandList.Get(),
		GameScreen::GetWidth() / 4, GameScreen::GetHeight() / 4,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*�ٸ�*/,
		D3D12_RESOURCE_STATE_COMMON/*�ٸ�*/,
		NULL/*, 0*/ /*�ε���*/);

	m_RWMiddleBloomTex = d3dUtil::CreateTexture2DResource
	(m_d3dDevice.Get(), m_CommandList.Get(),
		GameScreen::GetWidth() / 4, GameScreen::GetHeight() / 4,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*�ٸ�*/,
		D3D12_RESOURCE_STATE_COMMON/*�ٸ�*/,
		NULL/*, 0*/ /*�ε���*/);
	 
	m_RWBloomTex = d3dUtil::CreateTexture2DResource
		(m_d3dDevice.Get(), m_CommandList.Get(), 
			GameScreen::GetWidth() / 4, GameScreen::GetHeight() / 4,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*�ٸ�*/,
			D3D12_RESOURCE_STATE_COMMON/*�ٸ�*/,
			NULL/*, 0*/ /*�ε���*/);  

}

void CGameFramework::CreateRWBuffer()
{ 
	// Middle Avg Lum ............ ///////////////////
	UINT64 byteSize = NumMiddleAvgLum * sizeof(float);
	 
	// Create the buffer that will be a UAV.
	ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&m_RWMiddleAvgLum)));

	ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_ReadBackMiddleAvgLumBuffer)));
	// Middle Avg Lum ............ ///////////////////


	// Avg Lum ............ ///////////////////
	byteSize = NumAvgLum * sizeof(float);

	// Create the buffer that will be a UAV.
	ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&m_RWAvgLum)));

	ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_ReadBackAvgLumBuffer)));
	// Avg Lum ............ ///////////////////
}

void CGameFramework::ReleaseSwapChainBuffer()
{
	for (int i = 0; i < m_SwapChainBuffersCount; i++) {
		if (m_RenderTargetBuffers[i])
		{
			m_RenderTargetBuffers[i]->Release();
		}
	}
}

void CGameFramework::ReleaseGBuffers()
{
	if (m_GBufferHeap)
	{
		m_GBufferHeap->ReleaseObjects();
		delete m_GBufferHeap;
		m_GBufferHeap = nullptr;
	}

	for (int i = 0; i < m_GBuffersCountForRenderTarget; ++i)
	{
		if(m_GBuffersForRenderTarget[i]) m_GBuffersForRenderTarget[i]->Release();
	}
}

void CGameFramework::ReleaseDepthStencilBuffer()
{
	if (m_DepthStencilBuffer) {
		m_DepthStencilBuffer->Release();
	}
}

void CGameFramework::ReleaseShadowmap()
{
	if (m_GBufferHeap)
	{
		m_GBufferHeap->ReleaseObjects();
		delete m_GBufferHeap;
		m_GBufferHeap = nullptr;
	}

	m_Shadowmap->Release();
}

void CGameFramework::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	::ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = m_SwapChainBuffersCount;
	SwapChainDesc.BufferDesc.Width = GameScreen::GetClientWidth();
	SwapChainDesc.BufferDesc.Height = GameScreen::GetClientHeight();
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.OutputWindow = m_hWnd;
	SwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	SwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	SwapChainDesc.Windowed = TRUE;

#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	//��üȭ�� ��忡�� ����ȭ���� �ػ󵵸� �ٲ��� �ʰ� �ĸ������ ũ�⸦ ����ȭ�� ũ��� �����Ѵ�.
	SwapChainDesc.Flags = 0;
#else
	//��üȭ�� ��忡�� ����ȭ���� �ػ󵵸� ����ü��(�ĸ����)�� ũ�⿡ �°� �����Ѵ�.
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	//����ü���� �����Ѵ�.
	HRESULT hResult = m_dxgiFactory->CreateSwapChain(
		m_CommandQueue.Get(),
		&SwapChainDesc, 
		(IDXGISwapChain **)m_SwapChain.GetAddressOf());
	assert(hResult == S_OK);

	//��Alt+Enter�� Ű�� ������ ��Ȱ��ȭ�Ѵ�.
	m_dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

	//����ü���� ���� �ĸ���� �ε����� �����Ѵ�.
	m_SwapChainBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
}

void CGameFramework::CreateDirect3DDevice()
{
	HRESULT hResult;

	// ����� ���̾� ���� �����Ѵ�.
#if defined(_DEBUG)
	D3D12GetDebugInterface(IID_PPV_ARGS(&m_pd3dDebugController));
	m_pd3dDebugController->EnableDebugLayer(); 
#endif
	
	// DXGI ���丮�� �����Ѵ�.
	hResult = ::CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	assert(hResult == S_OK);

	//��� �ϵ���� ����� ���Ͽ� Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� �����Ѵ�.
	IDXGIAdapter1 *pAdapter = nullptr;
	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_dxgiFactory->EnumAdapters1(i, &pAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 AdapterDesc;
		pAdapter->GetDesc1(&AdapterDesc);
		if (AdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_d3dDevice)))) {
			break;
		}
	}
	
	//Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ �� ������ WARP ����̽��� �����Ѵ�.
	if (!pAdapter)
	{
		m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter));
		hResult = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3dDevice));
		assert(hResult == S_OK);
	}
	 
	// MSAA ǰ�� ���� ���� ���� Ȯ��
	// ����̽��� �����ϴ� ���� ������ ǰ�� ������ Ȯ���Ѵ�.
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels;
	MsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	MsaaQualityLevels.SampleCount = 4; //Msaa4x ���� ���ø�
	MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	MsaaQualityLevels.NumQualityLevels = 0;
	
	hResult = m_d3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&MsaaQualityLevels,
		sizeof(MsaaQualityLevels));
	assert(hResult == S_OK);

	m_nMsaa4xQualityLevels = MsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;
	//���� ������ ǰ�� ������ 1���� ũ�� ���� ���ø��� Ȱ��ȭ�Ѵ�.
	
	//�潺�� �����Ѵ�.
	hResult = m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
	assert(hResult == S_OK);
	
	//�潺�� ����ȭ�� ���� �̺�Ʈ ��ü�� �����Ѵ�(�̺�Ʈ ��ü�� �ʱⰪ�� FALSE�̴�).
	//�̺�Ʈ�� ����Ǹ�(Signal) �̺�Ʈ�� ���� �ڵ������� FALSE�� �ǵ��� �����Ѵ�.
	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	

	d3dUtil::gnCbvSrvDescriptorIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	d3dUtil::gnDsvDescriptorIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	if (pAdapter) pAdapter->Release();
}

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	HRESULT hResult;

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = m_SwapChainBuffersCount + (m_GBuffersCountForRenderTarget); 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // ���� Ÿ�� ��
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	//���� Ÿ�� ������ ��(�������� ������ ����ü�� ������ ����)�� �����Ѵ�.
	hResult = m_d3dDevice->CreateDescriptorHeap( &d3dDescriptorHeapDesc, IID_PPV_ARGS(&m_RtvHeap));
	assert(hResult == S_OK);

	//���� Ÿ�� ������ ���� ������ ũ�⸦ �����Ѵ�.
	m_RtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	//����-���ٽ� ������ ��(�������� ������ 1)�� �����Ѵ�.
	d3dDescriptorHeapDesc.NumDescriptors = m_DsvDescriptorsCount;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // ���� ���ٽ� �� 
	hResult = m_d3dDevice->CreateDescriptorHeap( &d3dDescriptorHeapDesc,  IID_PPV_ARGS(&m_DsvHeap)); assert(hResult == S_OK);

	//����-���ٽ� ������ ���� ������ ũ�⸦ �����Ѵ�.
	m_DsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void CGameFramework::CreateShadowmapView()
{
	// ���ҽ� ���� /////////////////////////////////////////////////
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = GameScreen::GetClientWidth();
	texDesc.Height = GameScreen::GetClientHeight();
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&optClear,
		IID_PPV_ARGS(&m_Shadowmap)));

	ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&optClear,
		IID_PPV_ARGS(&m_PlayerShadowmap)));
	
	//// ���ҽ� ���� /////////////////////////////////////////////////

	// ��ũ���� ���� /////////////////////////////////////////////
	m_ShadowmapHeap = new MyDescriptorHeap();
	m_ShadowmapHeap->CreateCbvSrvUavDescriptorHeaps(m_d3dDevice.Get(), m_CommandList.Get(), 0, m_ShadowmapCount, 0, ENUM_SCENE::SCENE_NONE);

	 // Create SRV to resource so we can sample the shadow map in a shader program.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice = 0;

	auto hCpuSrvForShadow = m_ShadowmapHeap->GetCPUDescriptorHandleForHeapStart(); // ���̴� ���ҽ� �� 
	m_d3dDevice->CreateShaderResourceView(m_Shadowmap, &srvDesc, hCpuSrvForShadow);

	hCpuSrvForShadow.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	m_d3dDevice->CreateShaderResourceView(m_PlayerShadowmap, &srvDesc, hCpuSrvForShadow); 
	 
	auto hCpuDsvForShadow = m_DsvHeap->GetCPUDescriptorHandleForHeapStart(); // 0: �⺻ depth stencil
	hCpuDsvForShadow.ptr += d3dUtil::gnDsvDescriptorIncrementSize; // 1: gbuffer detph
	hCpuDsvForShadow.ptr += d3dUtil::gnDsvDescriptorIncrementSize; // 2: Shadow
	m_ShadowmapCPUHandle = hCpuDsvForShadow;
	hCpuDsvForShadow.ptr += d3dUtil::gnDsvDescriptorIncrementSize; // 3:Player Shaodw
	m_PlayerShadowmapCPUHandle = hCpuDsvForShadow;

	// Create DSV to resource so we can render to the shadow map.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.Texture2D.MipSlice = 0;

	m_d3dDevice->CreateDepthStencilView(m_Shadowmap, &dsvDesc, m_ShadowmapCPUHandle);
	m_d3dDevice->CreateDepthStencilView(m_PlayerShadowmap, &dsvDesc, m_PlayerShadowmapCPUHandle);
	
	// ��ũ���� ���� /////////////////////////////////////////////
	   
}

void CGameFramework::CreateCommandQueueAndList()
{
	HRESULT hResult;

	//����(Direct) ��� ť�� �����Ѵ�.
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hResult = m_d3dDevice->CreateCommandQueue( &d3dCommandQueueDesc, IID_PPV_ARGS(&m_CommandQueue));
	assert(hResult == S_OK);

	//����(Direct) ��� �Ҵ��ڸ� �����Ѥ���.
	hResult = m_d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_CommandAllocator));
	assert(hResult == S_OK);

	//����(Direct) ��� ����Ʈ�� �����Ѵ�.
	hResult = m_d3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_CommandAllocator.Get(),
		NULL, 
		IID_PPV_ARGS(&m_CommandList));
	assert(hResult == S_OK);

	//��� ����Ʈ�� �����Ǹ� ����(Open) �����̹Ƿ� ����(Closed) ���·� �����.
	hResult = m_CommandList->Close();
	assert(hResult == S_OK);
}

void CGameFramework::CreateRenderTargetView()
{
	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
	d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // r,g,b 8bit 0~1
	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < m_SwapChainBuffersCount; i++)
	{
		m_SwapChainCPUHandle[i] = d3dRtvCPUDescriptorHandle;
		HRESULT hResult = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargetBuffers[i]));

		m_d3dDevice->CreateRenderTargetView(m_RenderTargetBuffers[i], &d3dRenderTargetViewDesc, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += m_RtvDescriptorSize;
	}
}

void CGameFramework::CreateDepthStencilView()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.Alignment = 0;
	ResourceDesc.Width = GameScreen::GetClientWidth();
	ResourceDesc.Height = GameScreen::GetClientHeight();
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ResourceDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	ResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	
	D3D12_HEAP_PROPERTIES HeapProperties;
	::ZeroMemory(&HeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProperties.CreationNodeMask = 1;
	HeapProperties.VisibleNodeMask = 1;
	
	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ClearValue.DepthStencil.Depth = 1.0f;
	ClearValue.DepthStencil.Stencil = 0;

	//����-���ٽ� ���۸� �����Ѵ�.

	// ���� ���ٽ� ���ҽ� ����
	m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue, IID_PPV_ARGS(&m_DepthStencilBuffer));
	
	m_DepthStencilCPUHandle = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();

	m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer, &d3dDepthStencilViewDesc, m_DepthStencilCPUHandle);

	// GBuffer ///////////////////////////////////////////////
	//// ���� ���ٽ� �� ����
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; 

	m_GBufferCPUHandleForDepth[0] = m_DepthStencilCPUHandle;
	m_GBufferCPUHandleForDepth[0].ptr += m_DsvDescriptorSize; // 2��° ���� ���ٽ� ��.
	
	// GBUFFER ���� ���ҽ� �ڿ� ����
	m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue, IID_PPV_ARGS(&m_GBuffersForDepth[0]));

	m_d3dDevice->CreateDepthStencilView(m_GBuffersForDepth[0], &d3dDepthStencilViewDesc, m_GBufferCPUHandleForDepth[0]);
	
	// ���� �߰�
	m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_GBuffersForDepth[0], RESOURCE_TEXTURE2D, m_GBufferForDepthIndex, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

}

void CGameFramework::CreateGBufferView()
{ 
	for (UINT i = 0; i < m_GBuffersCountForRenderTarget; i++)
	{ 
		DXGI_FORMAT textureType ;
		D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		 
		if (i == 0)
		{
			textureType = DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		else if (i == 1) // �븻��
		{
			textureType = DXGI_FORMAT_R11G11B10_FLOAT; // 32BIT ��� 

		}
		else
		{
			textureType = DXGI_FORMAT_R8G8B8A8_UNORM; 
			 
		}
		D3D12_CLEAR_VALUE d3dClearValue = { textureType,
		{m_clearValueColor[0], m_clearValueColor[1], m_clearValueColor[2], m_clearValueColor[3] } };
		
		// �ش� �ؽ��ĸ� 2�� �ؽ��ķ� ���� ��´�.
		m_GBuffersForRenderTarget[i] =
			d3dUtil::CreateTexture2DResource(
				m_d3dDevice.Get(), m_CommandList.Get(),
				GameScreen::GetClientWidth(), GameScreen::GetClientHeight(),
				textureType, // 32bit 
				flag,
				D3D12_RESOURCE_STATE_GENERIC_READ, &d3dClearValue
			);
	}

	// ��ũ���� �ڵ� ����
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_SwapChainBuffersCount * m_RtvDescriptorSize); // SWAP CHAIN ���ķ� �����ϱ� ����...

	// ����Ÿ�� �� ����
	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
	d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;

	for (UINT i = 0; i < m_GBuffersCountForRenderTarget; i++)
	{ 
		// ����Ÿ�� �� ����
		m_GBufferCPUHandleForRenderTarget[i] = d3dRtvCPUDescriptorHandle;
		m_GBufferCPUHandleForRenderTarget[i].ptr += (m_RtvDescriptorSize * i);

		if (i == 0)
		{
			d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		else if (i == 1)
		{
			d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
		}
		else
		{
			d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		// ����̽��� ����Ÿ�ٺ並 �ش� �ؽ��ķ� �����ȴ�...
		m_d3dDevice->CreateRenderTargetView(m_GBuffersForRenderTarget[i], &d3dRenderTargetViewDesc, m_GBufferCPUHandleForRenderTarget[i]);
	}

	if (!m_GBufferHeap)
	{
		m_GBufferHeap = new MyDescriptorHeap();
		m_GBufferHeap->CreateCbvSrvUavDescriptorHeaps(m_d3dDevice.Get(), m_CommandList.Get(), 0, m_GBufferHeapCount, m_PostProcessingCount, ENUM_SCENE::SCENE_NONE);
		for (int i = 0; i < m_GBuffersCountForRenderTarget; ++i)
		{
			m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_GBuffersCountForRenderTarget, m_GBuffersForRenderTarget[i], RESOURCE_TEXTURE2D, i);
		} 

		// ������ �ʵ� ���� �߰��Ѵ�...!!!
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_Shadowmap, RESOURCE_TEXTURE2D, m_GBufferForShadowIndex, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// ������ �ʵ� ���� �߰��Ѵ�...!!!
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_PlayerShadowmap, RESOURCE_TEXTURE2D, m_GBufferForPlayerShadowIndex, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		auto resourceDesc = m_RWBloomTex->GetDesc();

		// ���� uav �� srv�� ���� �߰�
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_RWHDRTex_1_16, RESOURCE_TEXTURE2D, m_GBufferForHDR1_16, resourceDesc.Format);
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_RWMiddleBloomTex, RESOURCE_TEXTURE2D, m_GBufferForMiddleBloom, resourceDesc.Format);
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_RWBloomTex, RESOURCE_TEXTURE2D, m_GBufferForBloom, resourceDesc.Format);
		 
		// ���� �� uav�� ���� �߰��Ѵ�...
		m_GBufferHeap->CreateUnorderedAccessViews(m_d3dDevice.Get(), m_CommandList.Get(), m_RWHDRTex_1_16, RESOURCE_TEXTURE2D, 0, resourceDesc.Format);
		m_GBufferHeap->CreateUnorderedAccessViews(m_d3dDevice.Get(), m_CommandList.Get(), m_RWMiddleBloomTex, RESOURCE_TEXTURE2D, 1, resourceDesc.Format);
		m_GBufferHeap->CreateUnorderedAccessViews(m_d3dDevice.Get(), m_CommandList.Get(), m_RWBloomTex, RESOURCE_TEXTURE2D, 2, resourceDesc.Format);
	} 
}

void CGameFramework::BuildObjects()
{
	HRESULT hResult;

	m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	
	///////////////////////////////////////////////////////////////////////////// ���ҽ� ����
	// ���� ���� X /////////////

	GraphicsRootSignatureMgr::BuildObject(m_d3dDevice.Get()); // ��Ʈ �ñ״�ó ���� ����
	BuildShaders(); // ������ ��Ʈ �ñ״�ó�� ����ϴ� ���̴� ����
	CreateRWBuffer(); // CommandList Reset�� ���Ŀ� �ؾ���
	
	SkillStg::GetInstance()->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get()); // ��ų ����Ʈ ����
	EffectMgr::GetInstance()->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get());

	m_SceneMgr = new SceneMgr; 

	// �ͷ��� ������Ʈ���� ���� ��� �ؽ��İ� ���� �����´�.
	TextureStorage::GetInstance()->CreateTextures(m_d3dDevice.Get(), m_CommandList.Get());
	ModelStorage::GetInstance()->CreateModels(m_d3dDevice.Get(), m_CommandList.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	 
	// ��� ���� ������Ʈ �� �ؽ��ĵ��� �����մϴ�.
	m_SceneMgr->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get());

	// ����ǰ� ����� �ؽ��� ������ŭ SRV ���� �����ϰ�, �� ��� �ؽ��ĵ��� �����մϴ�.
	m_SceneMgr->BuildHeap(m_d3dDevice.Get(), m_CommandList.Get());

	// �ͷ����� ���� ������ �ʰ� �� ����...
	GameScene::CreateSrvDescriptorHeapsForShadowmap(m_d3dDevice.Get(), m_CommandList.Get(), m_Shadowmap);
	GameScene::CreateSrvDescriptorHeapsForPlayerShadowmap(m_d3dDevice.Get(), m_CommandList.Get(), m_PlayerShadowmap);

	// ���� ���� X /////////////
	///////////////////////////////////////////////////////////////////////////// ���ҽ� ����

	hResult = m_CommandList->Close();
	assert(hResult == S_OK);
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();

	//// ���ε� �� ������ ///////////////////////////////////////////////////////////////////////// 
	if (m_SceneMgr) m_SceneMgr->ReleaseUploadBuffers();

	SkillStg::GetInstance()->ReleaseUploadBuffers();
	TextureStorage::GetInstance()->ReleaseUploadBuffers();
	ModelStorage::GetInstance()->ReleaseUploadBuffers();
	EffectMgr::GetInstance()->ReleaseUploadBuffers();
	//// ���ε� �� ������ ///////////////////////////////////////////////////////////////////////// 


	CGameTimer::GetInstance()->Reset();
}

void CGameFramework::ReleaseObjects()
{ 
	if (m_SceneMgr)
	{ 
		m_SceneMgr->ReleaseObjects();
		delete m_SceneMgr;
		m_SceneMgr = nullptr;
	}

	m_verticalShader->ReleaseObjects();
	delete m_verticalShader;
	m_verticalShader = nullptr;

	m_horizenShader->ReleaseObjects();
	delete m_horizenShader;
	m_horizenShader = nullptr;

	SkillStg::GetInstance()->ReleaseObjects();

	CMaterial::ReleaseShaders(); 
	StaticObjectStorage::GetInstance()->ReleaseObjects();
	TextureStorage::GetInstance()->ReleaseObjects();
	ModelStorage::GetInstance()->ReleaseObjects();
	ShaderManager::GetInstance()->ReleaseObjects();
	EffectMgr::GetInstance()->ReleaseObjects();

	StaticObjectStorage::ReleaseInstance();
	ShaderManager::ReleaseInstance();
	TextureStorage::ReleaseInstance();
	ModelStorage::ReleaseInstance();
	EffectMgr::ReleaseInstance();

}

void CGameFramework::UpdateGamelogic(float fElapsedTime)
{
	if (m_SceneMgr)
	{
		GameInput::Update(m_hWnd);
		m_SceneMgr->GetCurrScene()->ProcessInput(m_hWnd, fElapsedTime);
		m_SceneMgr->GetCurrScene()->UpdatePhysics(fElapsedTime);
		m_SceneMgr->GetCurrScene()->Update(fElapsedTime);
		m_SceneMgr->GetCurrScene()->AnimateObjects(fElapsedTime);
		m_SceneMgr->GetCurrScene()->LastUpdate(fElapsedTime); 
		GameInput::Reset();
	}
}

void CGameFramework::WaitForGpuComplete()
{
	const UINT64 nFenceValue = ++m_nFenceValues[m_SwapChainBufferIndex];
	HRESULT hResult = m_CommandQueue->Signal(m_Fence.Get(), nFenceValue);

	if (m_Fence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_Fence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CGameFramework::MoveToNextFrame()
{
	m_SwapChainBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();

	UINT64 nFenceValue = ++m_nFenceValues[m_SwapChainBufferIndex];
	HRESULT hResult = m_CommandQueue->Signal(m_Fence.Get(), nFenceValue);

	if (m_Fence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_Fence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_SceneMgr) 
	{
		m_SceneMgr->GetCurrScene()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	}

	switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		GameInput::RotateWheel(wParam);
		break;
	case WM_LBUTTONDOWN:
		//���콺 ĸ�ĸ� �ϰ� ���� ���콺 ��ġ�� �����´�. 
		GameInput::SetCapture(hWnd);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		//���콺 ĸ�ĸ� �����Ѵ�.
		GameInput::ReleaseCapture();
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE: 
		GameInput::MouseMove(lParam);
		break;

	default:
		break;
	}
}

static BOOL is_fullscreen = FALSE;
void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_SceneMgr) 
	{
		m_SceneMgr->GetCurrScene()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam, 0.f);
	}

	bool isStateChange = false;
	switch (nMessageID)
	{
	case WM_KEYDOWN:    
		switch (wParam)
		{
		case VK_F5: // Main Scene ���� ��ȯ
		{
			isStateChange = true;
			m_SceneMgr->ChangeSceneToMain();
			break;
		}
		case VK_F6: // Skill Select Scene ���� ��ȯ
		{
			isStateChange = true;
			m_SceneMgr->ChangeSceneToSkillSelect();
			break;
		}
		case VK_F7: // Game Scene ���� ��ȯ
		{
			isStateChange = true;
			m_SceneMgr->ChangeSceneToGame();
			break;
		}
		case VK_F8: // Win Scene ���� ��ȯ
		{
			isStateChange = true;
			m_SceneMgr->ChangeSceneToWin();
			break;
		}
		case VK_F9: // Lose Scene ���� ��ȯ
		{
			isStateChange = true;
			m_SceneMgr->ChangeSceneToLose();
			break;
		}
		default:
			break;
		}
		break;
	case WM_KEYUP: 
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		
		case VK_F1:
		{ 
			if (!is_fullscreen)
			{
				is_fullscreen = TRUE;
				GameScreen::ChangeFullScreen(GameScreen::GetWidth(), GameScreen::GetHeight());
			}
			else
			{
				is_fullscreen = FALSE;
				GameScreen::ChangeWindowScreen(GameScreen::GetWidth(), GameScreen::GetHeight());
			} 
			OnResizeBackBuffers();
			break;
		} 

		case VK_F5:  
		case VK_F6:  
		case VK_F7:  
		case VK_F8:  
		case VK_F9:  
		{
			isStateChange = true; 
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}

	// ���� F8 ~ F12�� ��� ��ȯ�� ��� �Ʒ��� �������� �ʴ´�.
	if (isStateChange) return;

	// ���� �ƹ�Ű�� ������ ��� ��ȯ ///////////////////
	if (m_SceneMgr->IsMainScene())
	{
		m_SceneMgr->ChangeSceneToSkillSelect();
	}
	else if (m_SceneMgr->IsSkillSelectScene())
	{
		m_SceneMgr->ChangeSceneToGame();
	}
}
 
void CGameFramework::DefferedRenderOnSwapchain()
{
	RenderOnRT(&CGameFramework::DefferedRenderSwapChain, m_RenderTargetBuffers[m_SwapChainBufferIndex], m_SwapChainCPUHandle[m_SwapChainBufferIndex], m_DepthStencilCPUHandle);
}

void CGameFramework::RenderShadowMap()
{
	////���������� ���¸� �����Ѵ�.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);

	float anotherWidth = static_cast<float>(GameScreen::GetClientWidth()) / 4;
	float anotherHeight = static_cast<float>(GameScreen::GetClientHeight()) / 4;

	float width = static_cast<float>(GameScreen::GetClientWidth());
	float height = static_cast<float>(GameScreen::GetClientHeight());
	 
	m_ShadowmapHeap->UpdateShaderVariable(m_CommandList.Get());

	// ����� �����մϴ�. 
	auto handle = m_ShadowmapHeap->GetGPUDescriptorHandleForHeapStart();

	// ����� �����մϴ�.
	for (int i = 0; i < m_ShadowmapCount; ++i)
	{
		D3D12_VIEWPORT	GBuffer_Viewport{ anotherWidth *  i, height - anotherHeight, anotherWidth , anotherHeight, 1.0f, 0.0f };
		D3D12_RECT		ScissorRect{ anotherWidth * i, height - anotherHeight, anotherWidth * (3 + i) , height + anotherHeight };

		m_CommandList->RSSetViewports(1, &GBuffer_Viewport);
		m_CommandList->RSSetScissorRects(1, &ScissorRect);
		 
		m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, handle);
		handle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->DrawInstanced(6, 1, 0, 0);

	} 
}

void CGameFramework::BuildShaders()
{
	// compute shader ////////////////
	m_verticalShader = new VerticalBlurShader();
	m_verticalShader->CreateShader(m_d3dDevice.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	
	m_horizenShader = new HorizonBlurShader();
	m_horizenShader->CreateShader(m_d3dDevice.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature()); 
	
	m_downScaleFirstPassShader = new DownScaleFirstPassShader();
	m_downScaleFirstPassShader->CreateShader(m_d3dDevice.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	
	m_downScaleSecondPassShader = new DownScaleSecondPassShader();
	m_downScaleSecondPassShader->CreateShader(m_d3dDevice.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	
	m_BloomRevealShader = new BloomRevealShader();
	m_BloomRevealShader->CreateShader(m_d3dDevice.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// compute shader ///////////////

	CMaterial::PrepareShaders(m_d3dDevice.Get(), m_CommandList.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	ShaderManager::GetInstance()->BuildShaders(m_d3dDevice.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
}
 
void CGameFramework::RenderOnRT(renderFuncPtr renderPtr, ID3D12Resource* pRendertargetResource, D3D12_CPU_DESCRIPTOR_HANDLE hCPUrenderTarget, D3D12_CPU_DESCRIPTOR_HANDLE hCPUdepthStencil)
{
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), pRendertargetResource, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_CommandList->ClearRenderTargetView(hCPUrenderTarget, m_clearValueColor, 0, NULL);
	m_CommandList->ClearDepthStencilView(hCPUdepthStencil, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	m_CommandList->OMSetRenderTargets(1, &hCPUrenderTarget, TRUE, &hCPUdepthStencil);
	
	(this->*renderPtr)(); 
	 
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), pRendertargetResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void CGameFramework::RenderOnRTs(renderFuncPtr renderPtr, UINT RenderTargetCount, ID3D12Resource ** pRendertargetResources, D3D12_CPU_DESCRIPTOR_HANDLE * hCPUrenderTargets, D3D12_CPU_DESCRIPTOR_HANDLE hCPUdepthStencils)
{
	for (int i = 0; i < RenderTargetCount; ++i)
	{
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), pRendertargetResources[i], D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ClearRenderTargetView(hCPUrenderTargets[i], m_clearValueColor, 0, NULL);
	}

	m_CommandList->ClearDepthStencilView(hCPUdepthStencils, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	m_CommandList->OMSetRenderTargets(RenderTargetCount, hCPUrenderTargets, TRUE, &hCPUdepthStencils);

	(this->*renderPtr)();

	for (int i = 0; i < RenderTargetCount; ++i)
	{
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), pRendertargetResources[i], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	}
}

void CGameFramework::RenderOnGbuffer()
{  
	if (m_SceneMgr)
	{
		m_SceneMgr->GetCurrScene()->Render(m_CommandList.Get(), true);
	} 
}
 
LRESULT CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			CGameTimer::GetInstance()->Stop();
		else
			CGameTimer::GetInstance()->Start();
		break;
	}

	case WM_SIZE:
	{
		GameScreen::SetScreen(LOWORD(lParam) , HIWORD(lParam));

		//�������� ũ�Ⱑ ����Ǹ� �ĸ������ ũ�⸦ �����Ѵ�.
		OnResizeBackBuffers();
		break;
	}

	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_KEYDOWN: 
		switch (wParam) {
		case VK_SPACE:
			break;
		}
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::OnResizeBackBuffers()
{
	WaitForGpuComplete();

	m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	
	// ��������X ////////////
	ReleaseSwapChainBuffer();
	ReleaseGBuffers();
	ReleaseDepthStencilBuffer(); 
	// ��������X ////////////


#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	m_SwapChain->GetDesc(&SwapChainDesc);
	m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	m_SwapChainBufferIndex = 0;
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_SwapChain->GetDesc(&dxgiSwapChainDesc);
	m_SwapChain->ResizeBuffers(m_SwapChainBuffersCount, GameScreen::GetClientWidth(),
		GameScreen::GetClientHeight(), dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
	m_SwapChainBufferIndex = 0;
#endif

	// ��������X ////////////
	CreateRenderTargetView();
	CreateGBufferView();
	CreateDepthStencilView();  
	// ��������X ////////////
	

	m_CommandList->Close();
	
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();
}

void CGameFramework::Blur()
{
	// MiddleBloom �ؽ��ĸ� �����Ͽ� Bloom �ؽ��ķ� �����ϴ� PASS �Դϴ�. //////////////////////////////////

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWBloomTex, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// ���� �� //////////////////////////////////
	m_horizenShader->SetPSO(m_CommandList.Get()); 

	// ����� ���ҽ� ������Ʈ
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForMiddleBloom));
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(2));
	// ����� ���ҽ� ������Ʈ

	UINT groupX = (UINT)ceilf(float(GameScreen::GetWidth()) / 256.F / 4.f);
	m_CommandList->Dispatch(groupX, GameScreen::GetHeight() / 4.f, 1);
	// ���� �� //////////////////////////////////


	// ���� �� ////////////////////////////////// 
	m_verticalShader->SetPSO(m_CommandList.Get()); 

	// ����� ���ҽ� ������Ʈ
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForMiddleBloom));
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(2));
	// ����� ���ҽ� ������Ʈ

	UINT groupY = (UINT)ceilf(GameScreen::GetHeight() / 256.F / 4.f);
	m_CommandList->Dispatch(GameScreen::GetWidth() / 4.f, groupY, 1);
	// ���� �� //////////////////////////////////

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWBloomTex, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON);
}

void CGameFramework::Bloom()
{   
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWHDRTex_1_16, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// Bloom �� ���� �ֵ����� MiddleBloom �ؽ��Ŀ� �����ϴ� PASS //////////////////////////////////
	m_BloomRevealShader->SetPSO(m_CommandList.Get());

	// ����� ���ҽ� ������Ʈ
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForHDR1_16));
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(1));
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());
	// ����� ���ҽ� ������Ʈ

	UINT groupX = (UINT)ceil((float)((GameScreen::GetWidth() * GameScreen::GetHeight()) / 16.f) / 1024.f);
	m_CommandList->Dispatch(groupX, 1, 1);
	// Bloom �� ���� �ֵ����� MiddleBloom �ؽ��Ŀ� �����ϴ� PASS //////////////////////////////////
	  
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWHDRTex_1_16, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON);
}

void CGameFramework::DownScale()
{  
	// �ٿ� ������ ù��° PASS //////////////////////////////////
	m_downScaleFirstPassShader->SetPSO(m_CommandList.Get());

	// ����� ���ҽ� ������Ʈ
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUDescriptorHandleForHeapStart());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(0));
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());
	// ����� ���ҽ� ������Ʈ

	UINT groupX = (UINT)ceil((float)((GameScreen::GetWidth() * GameScreen::GetHeight()) / 16.f) / 1024.f);
	m_CommandList->Dispatch(groupX, 1, 1);
    // �ٿ� ������ ù��° PASS //////////////////////////////////


	// �ٿ� ������ �ι�° PASS //////////////////////////////////
	m_downScaleSecondPassShader->SetPSO(m_CommandList.Get());

	// ����� ���ҽ� ������Ʈ 
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());
	// ����� ���ҽ� ������Ʈ
	 
	m_CommandList->Dispatch(1, 1, 1);
	// �ٿ� ������ �ι�° PASS //////////////////////////////////


	// ���ҽ� ī�� 
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWMiddleAvgLum, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));
	m_CommandList->CopyResource(m_ReadBackMiddleAvgLumBuffer, m_RWMiddleAvgLum);
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWMiddleAvgLum, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWAvgLum, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));
	m_CommandList->CopyResource(m_ReadBackAvgLumBuffer, m_RWAvgLum);
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWAvgLum, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
	// ���ҽ� ī��  

}
 
void CGameFramework::RenderSwapChain()
{  
	// ����� �����մϴ�.
	if (m_SceneMgr->GetCurrScene())
	{ 
		m_SceneMgr->GetCurrScene()->Render(m_CommandList.Get(), false);
	}

}

void CGameFramework::DefferedRenderSwapChain()
{ 
	//////���������� ���¸� �����Ѵ�.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_DEFFREDRENDER, false);
	
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	 
	MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(m_CommandList.Get());
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(m_CommandList.Get(), ROOTPARAMETER_CAMERA);
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateLightShaderVariables(m_CommandList.Get(), &LightManager::m_pLights->m_pLights[2]);

	//// ���ҽ��� �ٲ۴�.. 
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_GBufferHeap->GetGPUDescriptorHandleForHeapStart();

	D3D12_VIEWPORT	Viewport{ 0.f, 0.f, GameScreen::GetWidth() , GameScreen::GetHeight(), 1.0f, 0.0f };
	D3D12_RECT		ScissorRect{ 0, 0, GameScreen::GetWidth() , GameScreen::GetHeight() };

	m_CommandList->RSSetViewports(1, &Viewport);
	m_CommandList->RSSetScissorRects(1, &ScissorRect);

	for (int i = 0; i < 4; ++i)
	{
		m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_ALBEDO + i, handle);
		handle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	} 
	m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_SHADOWTEXTURE, handle); // �������� �������
	
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);
	 
}

void CGameFramework::RenderToTexture()
{
	//////���������� ���¸� �����Ѵ�.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);

	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

	MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(m_CommandList.Get());
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(m_CommandList.Get(), ROOTPARAMETER_CAMERA);
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateLightShaderVariables(m_CommandList.Get(), &LightManager::m_pLights->m_pLights[2]);

	//// ���ҽ��� �ٲ۴�.. 
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_GBufferHeap->GetGPUDescriptorHandleForHeapStart(); // 0��° ���ҽ�
	handle.ptr = handle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize * (m_GBufferForBloom);

	D3D12_VIEWPORT	Viewport{ 0.f, 0.f, GameScreen::GetWidth() , GameScreen::GetHeight(), 1.0f, 0.0f };
	D3D12_RECT		ScissorRect{ 0, 0, GameScreen::GetWidth() , GameScreen::GetHeight() };

	m_CommandList->RSSetViewports(1, &Viewport);
	m_CommandList->RSSetScissorRects(1, &ScissorRect);
	 
	m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, handle); 
	
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);
}

void CGameFramework::RenderForShadow()
{  
	if (m_SceneMgr->GetCurrScene())
	{
		m_SceneMgr->GetCurrScene()->RenderForShadow(m_CommandList.Get()); 
	} 
}

void CGameFramework::RenderForPlayerShadow()
{
	if (m_SceneMgr->GetCurrScene())
	{
		m_SceneMgr->GetCurrScene()->RenderForPlayerShadow(m_CommandList.Get());
	}
}

void CGameFramework::ToneMapping()
{
	//////���������� ���¸� �����Ѵ�.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_TONEMAPPING, false);

	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

	MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(m_CommandList.Get());
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(m_CommandList.Get(), ROOTPARAMETER_CAMERA);
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateLightShaderVariables(m_CommandList.Get(), &LightManager::m_pLights->m_pLights[2]);
	 
	m_CommandList->SetGraphicsRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetGraphicsRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());

	//// ���ҽ��� �ٲ۴�.. 
	
	D3D12_VIEWPORT	Viewport{ 0.f, 0.f, GameScreen::GetWidth() , GameScreen::GetHeight(), 1.0f, 0.0f };
	D3D12_RECT		ScissorRect{ 0, 0, GameScreen::GetWidth() , GameScreen::GetHeight() };

	m_CommandList->RSSetViewports(1, &Viewport);
	m_CommandList->RSSetScissorRects(1, &ScissorRect);

	m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(0));
	m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForBloom));

	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);

}
