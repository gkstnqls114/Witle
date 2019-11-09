#include "stdafx.h"
#include "d3dx12.h"
#include "d3dUtil.h"

//// GameBase ////////////////////////// 
#include "GameTimer.h"
#include "GameInput.h"
#include "GameScreen.h" 
//// GameBase ////////////////////////// 

//// Manager ////////////////////////// 
#include "BossSkillMgr.h"
#include "MainCameraMgr.h"
#include "GraphicsRootSignatureMgr.h"
#include "ShaderManager.h"
#include "ModelStorage.h"
#include "TextureStorage.h"
#include "StaticObjectStorage.h"
#include "SceneMgr.h"
#include "LightManager.h"
#include "SkillStg.h"
#include "HitEffectMgr.h"
#include "SoundManager.h"
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
#include "PostprocessingSetting.h"
//// ETC ////////////////////////////
 
#include "GameFramework.h"

void GameFramework::Render()
{
	HRESULT hResult = m_CommandAllocator->Reset();
	hResult = m_CommandList->Reset(m_CommandAllocator.Get(), NULL);

	m_CommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	m_CommandList->SetComputeRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	if (PostprocessingSetting::IsDefferedRendering())
	{
		// 쉐도우 맵을 그립니다.
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		RenderOnRTs(&GameFramework::RenderForShadow, 0, NULL, NULL, m_ShadowmapCPUHandle);
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
		// 쉐도우 맵을 그립니다.
		 
		// RenderOnGbuffer(); // G Buffer 에 렌더링합니다.
		RenderOnRTs(&GameFramework::RenderOnGbuffer, m_GBuffersCountForRenderTarget, m_GBuffersForRenderTarget, m_GBufferCPUHandleForRenderTarget, m_GBufferCPUHandleForDepth[0]);

		ToneCurveAndBloom();

		DefferedRenderOnSwapchain(); 
	}
	else
	{
		// 쉐도우 맵을 그립니다.
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		RenderOnRTs(&GameFramework::RenderForShadow, 0, NULL, NULL, m_ShadowmapCPUHandle);
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
		// 쉐도우 맵을 그립니다.

		// 플레이어 쉐도우 맵을 그립니다.
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_PlayerShadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		RenderOnRTs(&GameFramework::RenderForPlayerShadow, 0, NULL, NULL, m_PlayerShadowmapCPUHandle);
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_PlayerShadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
		// 쉐도우 맵을 그립니다.

		// 조명처리된 화면을 GBuffer에 그립니다. 그립니다.
		RenderOnRTs(&GameFramework::RenderSwapChain, 1, m_GBuffersForRenderTarget, m_GBufferCPUHandleForRenderTarget, m_GBufferCPUHandleForDepth[0]);

		// gbuffer heap을 설정합니다.
		m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

		// 휘도를 구합니다.
		DownScale();

		// bloom 을 위한 텍스쳐를 구합니다.
		Bloom();

		// Bloom 텍스쳐를 blur 합니다.
		Blur();

		// 톤매핑을 합니다.
		RenderOnRT(&GameFramework::ToneCurveAndBloom, m_RenderTargetBuffers[m_SwapChainBufferIndex], m_SwapChainCPUHandle[m_SwapChainBufferIndex], m_DepthStencilCPUHandle);
	}

	//// SwapChain에 Render //////////////////////////


	hResult = m_CommandList->Close();
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	m_SwapChain->Present(0, 0);

	MoveToNextFrame();
}

void GameFramework::Debug()
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

GameFramework::GameFramework()
{ 
}

GameFramework::~GameFramework()
{

}

bool GameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;
	GameInput::SetHWND(hMainWnd);

	// 절대 순서 변경 금지 ////////////
	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	CreateShadowmapView();
	CreateRWResourceViews();
	// 절대 순서 변경 금지 ////////////

	BuildObjects();
	
	GameScreen::SetSwapChain(m_SwapChain.Get());

	return true;
}

void GameFramework::OnDestroy()
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

}

void GameFramework::RenderGBuffers()
{ 
	m_CommandList->OMSetRenderTargets(1, &m_SwapChainCPUHandle[m_SwapChainBufferIndex], TRUE, &m_DepthStencilCPUHandle);

	////그래픽 루트 시그너쳐를 설정한다.
	m_CommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	////파이프라인 상태를 설정한다.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);
	
	float anotherWidth = static_cast<float>(GameScreen::GetClientWidth()) / 4;
	float anotherHeight = static_cast<float>(GameScreen::GetClientHeight()) / 4;

	float width = static_cast<float>(GameScreen::GetClientWidth());
	float height = static_cast<float>(GameScreen::GetClientHeight());
	 
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

	// 리소스만 바꾼다.. 
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_GBufferHeap->GetGPUDescriptorHandleForHeapStart();
	
	// 장면을 렌더합니다.
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


void GameFramework::CreateRWResourceViews()
{ 
	// 리소스 생성
	// 버퍼도 아니고, 리소스 상태가 깊이 스텐실도 렌더 타겟도 아니라면 클리어밸류는 NULL

	m_RWHDRTex_1_16 = d3dUtil::CreateTexture2DResource
	(m_d3dDevice.Get(), m_CommandList.Get(),
		GameScreen::GetWidth() / 4, GameScreen::GetHeight() / 4,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*다름*/,
		D3D12_RESOURCE_STATE_COMMON/*다름*/,
		NULL/*, 0*/ /*인덱스*/);

	m_RWMiddleBloomTex = d3dUtil::CreateTexture2DResource
	(m_d3dDevice.Get(), m_CommandList.Get(),
		GameScreen::GetWidth() / 4, GameScreen::GetHeight() / 4,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*다름*/,
		D3D12_RESOURCE_STATE_COMMON/*다름*/,
		NULL/*, 0*/ /*인덱스*/);
	 
	m_RWBloomTex = d3dUtil::CreateTexture2DResource
		(m_d3dDevice.Get(), m_CommandList.Get(), 
			GameScreen::GetWidth() / 4, GameScreen::GetHeight() / 4,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*다름*/,
			D3D12_RESOURCE_STATE_COMMON/*다름*/,
			NULL/*, 0*/ /*인덱스*/);  

}

void GameFramework::CreateRWBuffer()
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

void GameFramework::ReleaseSwapChainBuffer()
{ 
	for (int i = 0; i < m_SwapChainBuffersCount; ++i) 
	{
		if (m_RenderTargetBuffers[i])
		{
			m_RenderTargetBuffers[i]->Release();
			m_RenderTargetBuffers[i] = nullptr;
		}
	}
}

void GameFramework::ReleaseGBuffers()
{
	if (m_GBufferHeap)
	{
		m_GBufferHeap->ReleaseObjects();
		delete m_GBufferHeap;
		m_GBufferHeap = nullptr;
	}

	for (int i = 0; i < m_GBuffersCountForRenderTarget; ++i)
	{
		if (m_GBuffersForRenderTarget[i])
		{
			m_GBuffersForRenderTarget[i]->Release();
			m_GBuffersForRenderTarget[i] = nullptr;
		}
	}
}

void GameFramework::ReleaseDepthStencilBuffer()
{ 
	for (int i = 0; i < m_GBuffersCountForDepth; ++i)
	{
		if (m_GBuffersForDepth[i])
		{
			m_GBuffersForDepth[i]->Release();
			m_GBuffersForDepth[i] = nullptr;
		}
	} 

	if (m_DepthStencilBuffer)
	{
		m_DepthStencilBuffer->Release();
		m_DepthStencilBuffer = nullptr;
	} 
}
 
void GameFramework::CreateSwapChain()
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
	//전체화면 모드에서 바탕화면의 해상도를 바꾸지 않고 후면버퍼의 크기를 바탕화면 크기로 변경한다.
	SwapChainDesc.Flags = 0;
#else
	//전체화면 모드에서 바탕화면의 해상도를 스왑체인(후면버퍼)의 크기에 맞게 변경한다.
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	//스왑체인을 생성한다.
	HRESULT hResult = m_dxgiFactory->CreateSwapChain(
		m_CommandQueue.Get(),
		&SwapChainDesc, 
		(IDXGISwapChain **)m_SwapChain.GetAddressOf());
	assert(hResult == S_OK);

	//“Alt+Enter” 키의 동작을 비활성화한다.
	m_dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

	//스왑체인의 현재 후면버퍼 인덱스를 저장한다.
	m_SwapChainBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
}

void GameFramework::CreateDirect3DDevice()
{
	HRESULT hResult;

	// 디버그 레이어 층을 생성한다.
#if defined(_DEBUG)
	D3D12GetDebugInterface(IID_PPV_ARGS(&m_pd3dDebugController));
	m_pd3dDebugController->EnableDebugLayer(); 
#endif
	
	// DXGI 팩토리를 생성한다.
	hResult = ::CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	assert(hResult == S_OK);

	//모든 하드웨어 어댑터 대하여 특성 레벨 12.0을 지원하는 하드웨어 디바이스를 생성한다.
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
	
	//특성 레벨 12.0을 지원하는 하드웨어 디바이스를 생성할 수 없으면 WARP 디바이스를 생성한다.
	if (!pAdapter)
	{
		m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter));
		hResult = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3dDevice));
		assert(hResult == S_OK);
	}
	 
	// MSAA 품질 수준 지원 여부 확인
	// 디바이스가 지원하는 다중 샘플의 품질 수준을 확인한다.
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels;
	MsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	MsaaQualityLevels.SampleCount = 4; //Msaa4x 다중 샘플링
	MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	MsaaQualityLevels.NumQualityLevels = 0;
	
	hResult = m_d3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&MsaaQualityLevels,
		sizeof(MsaaQualityLevels));
	assert(hResult == S_OK);

	m_nMsaa4xQualityLevels = MsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;
	//다중 샘플의 품질 수준이 1보다 크면 다중 샘플링을 활성화한다.
	
	//펜스를 생성한다.
	hResult = m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
	assert(hResult == S_OK);
	
	//펜스와 동기화를 위한 이벤트 객체를 생성한다(이벤트 객체의 초기값을 FALSE이다).
	//이벤트가 실행되면(Signal) 이벤트의 값을 자동적으로 FALSE가 되도록 생성한다.
	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	

	d3dUtil::gnCbvSrvDescriptorIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	d3dUtil::gnDsvDescriptorIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	if (pAdapter) pAdapter->Release();
}

void GameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	HRESULT hResult;

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = m_SwapChainBuffersCount + (m_GBuffersCountForRenderTarget); 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // 렌더 타겟 뷰
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	//렌더 타겟 서술자 힙(서술자의 개수는 스왑체인 버퍼의 개수)을 생성한다.
	hResult = m_d3dDevice->CreateDescriptorHeap( &d3dDescriptorHeapDesc, IID_PPV_ARGS(&m_RtvHeap));
	assert(hResult == S_OK);

	//렌더 타겟 서술자 힙의 원소의 크기를 저장한다.
	m_RtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	//깊이-스텐실 서술자 힙(서술자의 개수는 1)을 생성한다.
	d3dDescriptorHeapDesc.NumDescriptors = m_DsvDescriptorsCount;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // 깊이 스텐실 뷰 
	hResult = m_d3dDevice->CreateDescriptorHeap( &d3dDescriptorHeapDesc,  IID_PPV_ARGS(&m_DsvHeap)); assert(hResult == S_OK);

	//깊이-스텐실 서술자 힙의 원소의 크기를 저장한다.
	m_DsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void GameFramework::CreateShadowmapView()
{
	// 리소스 생성 /////////////////////////////////////////////////
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
	
	//// 리소스 생성 /////////////////////////////////////////////////

	// 디스크립터 생성 /////////////////////////////////////////////
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

	auto hCpuSrvForShadow = m_ShadowmapHeap->GetCPUDescriptorHandleForHeapStart(); // 쉐이더 리소스 뷰 
	m_d3dDevice->CreateShaderResourceView(m_Shadowmap, &srvDesc, hCpuSrvForShadow);

	hCpuSrvForShadow.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	m_d3dDevice->CreateShaderResourceView(m_PlayerShadowmap, &srvDesc, hCpuSrvForShadow); 
	 
	auto hCpuDsvForShadow = m_DsvHeap->GetCPUDescriptorHandleForHeapStart(); // 0: 기본 depth stencil
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
	
	// 디스크립터 생성 /////////////////////////////////////////////
	   
}

void GameFramework::CreateCommandQueueAndList()
{
	HRESULT hResult;

	//직접(Direct) 명령 큐를 생성한다.
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hResult = m_d3dDevice->CreateCommandQueue( &d3dCommandQueueDesc, IID_PPV_ARGS(&m_CommandQueue));
	assert(hResult == S_OK);

	//직접(Direct) 명령 할당자를 생성한ㄹ다.
	hResult = m_d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_CommandAllocator));
	assert(hResult == S_OK);

	//직접(Direct) 명령 리스트를 생성한다.
	hResult = m_d3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_CommandAllocator.Get(),
		NULL, 
		IID_PPV_ARGS(&m_CommandList));
	assert(hResult == S_OK);

	//명령 리스트는 생성되면 열린(Open) 상태이므로 닫힌(Closed) 상태로 만든다.
	hResult = m_CommandList->Close();
	assert(hResult == S_OK);
}

void GameFramework::CreateRenderTargetView()
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

void GameFramework::CreateDepthStencilView()
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

	//깊이-스텐실 버퍼를 생성한다.

	// 깊이 스텐실 리소스 생성
	m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue, IID_PPV_ARGS(&m_DepthStencilBuffer));
	
	m_DepthStencilCPUHandle = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();

	m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer, &d3dDepthStencilViewDesc, m_DepthStencilCPUHandle);

	// GBuffer ///////////////////////////////////////////////
	//// 깊이 스텐실 뷰 생성
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; 

	m_GBufferCPUHandleForDepth[0] = m_DepthStencilCPUHandle;
	m_GBufferCPUHandleForDepth[0].ptr += m_DsvDescriptorSize; // 2번째 깊이 스텐실 뷰.
	
	// GBUFFER 깊이 리소스 자원 생성
	m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue, IID_PPV_ARGS(&m_GBuffersForDepth[0]));

	m_d3dDevice->CreateDepthStencilView(m_GBuffersForDepth[0], &d3dDepthStencilViewDesc, m_GBufferCPUHandleForDepth[0]);
	
	// 힙에 추가
	m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_GBuffersForDepth[0], RESOURCE_TEXTURE2D, m_GBufferForDepthIndex, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

}

void GameFramework::CreateGBufferView()
{ 
	for (UINT i = 0; i < m_GBuffersCountForRenderTarget; i++)
	{ 
		DXGI_FORMAT textureType ;
		D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		 
		if (i == 0)
		{
			textureType = DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		else if (i == 1) // 노말값
		{
			textureType = DXGI_FORMAT_R11G11B10_FLOAT; // 32BIT 노멀 

		}
		else
		{
			textureType = DXGI_FORMAT_R8G8B8A8_UNORM; 
			 
		}
		D3D12_CLEAR_VALUE d3dClearValue = { textureType,
		{m_clearValueColor[0], m_clearValueColor[1], m_clearValueColor[2], m_clearValueColor[3] } };
		
		// 해당 텍스쳐를 2디 텍스쳐로 만들어서 담는다.
		m_GBuffersForRenderTarget[i] =
			d3dUtil::CreateTexture2DResource(
				m_d3dDevice.Get(), m_CommandList.Get(),
				GameScreen::GetClientWidth(), GameScreen::GetClientHeight(),
				textureType, // 32bit 
				flag,
				D3D12_RESOURCE_STATE_GENERIC_READ, &d3dClearValue
			);
	}

	// 디스크립터 핸들 생성
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_SwapChainBuffersCount * m_RtvDescriptorSize); // SWAP CHAIN 이후로 설정하기 위해...

	// 렌더타겟 뷰 생성
	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
	d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;

	for (UINT i = 0; i < m_GBuffersCountForRenderTarget; i++)
	{ 
		// 렌더타겟 뷰 생성
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

		// 디바이스에 렌더타겟뷰를 해당 텍스쳐로 설정된다...
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

		// 쉐도우 맵도 힙에 추가한다...!!!
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_Shadowmap, RESOURCE_TEXTURE2D, m_GBufferForShadowIndex, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// 쉐도우 맵도 힙에 추가한다...!!!
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_PlayerShadowmap, RESOURCE_TEXTURE2D, m_GBufferForPlayerShadowIndex, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		auto resourceDesc = m_RWBloomTex->GetDesc();

		// 블러할 uav 를 srv로 힙에 추가
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_RWHDRTex_1_16, RESOURCE_TEXTURE2D, m_GBufferForHDR1_16, resourceDesc.Format);
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_RWMiddleBloomTex, RESOURCE_TEXTURE2D, m_GBufferForMiddleBloom, resourceDesc.Format);
		m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_RWBloomTex, RESOURCE_TEXTURE2D, m_GBufferForBloom, resourceDesc.Format);
		 
		// 블러를 할 uav도 힙에 추가한다...
		m_GBufferHeap->CreateUnorderedAccessViews(m_d3dDevice.Get(), m_CommandList.Get(), m_RWHDRTex_1_16, RESOURCE_TEXTURE2D, 0, resourceDesc.Format);
		m_GBufferHeap->CreateUnorderedAccessViews(m_d3dDevice.Get(), m_CommandList.Get(), m_RWMiddleBloomTex, RESOURCE_TEXTURE2D, 1, resourceDesc.Format);
		m_GBufferHeap->CreateUnorderedAccessViews(m_d3dDevice.Get(), m_CommandList.Get(), m_RWBloomTex, RESOURCE_TEXTURE2D, 2, resourceDesc.Format);
	} 
}

void GameFramework::BuildObjects()
{
	HRESULT hResult;

	m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	 
	///////////////////////////////////////////////////////////////////////////// 리소스 생성
	// 순서 변경 X /////////////

	GraphicsRootSignatureMgr::BuildObject(m_d3dDevice.Get()); // 루트 시그니처 먼저 생성
	BuildShaders(); // 생성된 루트 시그니처를 사용하는 쉐이더 생성
	CreateRWBuffer(); // CommandList Reset된 이후에 해야함
	 
	// 터레인 오브젝트에서 사용될 모든 텍스쳐과 모델을 가져온다.
	 TextureStorage::GetInstance()->CreateTextures(m_d3dDevice.Get(), m_CommandList.Get());

	//// 반드시 텍스쳐를 생성하고 나서 호출해야한다.
	ModelStorage::GetInstance()->CreateModels(m_d3dDevice.Get(), m_CommandList.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	HitEffectMgr::GetInstance()->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get());
	SkillStg::GetInstance()->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get()); // 스킬 이펙트 생성
	BossSkillMgr::GetInstance()->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get()); // 스킬 이펙트 생성

	// 모든 씬의 오브젝트 및 텍스쳐들을 빌드합니다.
	SceneMgr::GetInstance()->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get());

	// 빌드되고 연결된 텍스쳐 개수만큼 SRV 힙을 생성하고, 각 모든 텍스쳐들을 연결합니다.
	SceneMgr::GetInstance()->BuildHeap(m_d3dDevice.Get(), m_CommandList.Get());

	// 터레인을 위한 쉐도우 맵과 힙 연결...
	GameScene::CreateSrvDescriptorHeapsForShadowmap(m_d3dDevice.Get(), m_CommandList.Get(), m_Shadowmap);
	GameScene::CreateSrvDescriptorHeapsForPlayerShadowmap(m_d3dDevice.Get(), m_CommandList.Get(), m_PlayerShadowmap);

	// 순서 변경 X /////////////
	///////////////////////////////////////////////////////////////////////////// 리소스 생성

	hResult = m_CommandList->Close();
	assert(hResult == S_OK);
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();

	//// 업로드 힙 릴리즈 ///////////////////////////////////////////////////////////////////////// 
	SceneMgr::GetInstance()->ReleaseUploadBuffers(); 
	TextureStorage::GetInstance()->ReleaseUploadBuffers();
	ModelStorage::GetInstance()->ReleaseUploadBuffers();
	HitEffectMgr::GetInstance()->ReleaseUploadBuffers(); 
	BossSkillMgr::GetInstance()->ReleaseUploadBuffers(); 
	SkillStg::GetInstance()->ReleaseUploadBuffers();

	//// 업로드 힙 릴리즈 ///////////////////////////////////////////////////////////////////////// 


	CGameTimer::GetInstance()->Reset();
}

void GameFramework::ReleaseObjects()
{   
	GraphicsRootSignatureMgr::ReleaseObjects(); // 루트 시그니처 먼저 생성
	GameScene::ReleaseHeaps();

	if (m_GBufferHeap)
	{
		m_GBufferHeap->ReleaseObjects();
		delete m_GBufferHeap;
		m_GBufferHeap = nullptr;
	}

	for (int x = 0; x < m_GBuffersCountForRenderTarget; ++x)
	{
		if (m_GBuffersForRenderTarget[x])
		{
			m_GBuffersForRenderTarget[x]->Release();
		}
	}
	for (int x = 0; x < m_GBuffersCountForDepth; ++x)
	{
		if (m_GBuffersForDepth[x])
		{
			m_GBuffersForDepth[x]->Release();
		}
	}
	if (m_RWAvgLum)
	{
		m_RWAvgLum->Release();
	}
	if (m_RWMiddleAvgLum)
	{
		m_RWMiddleAvgLum->Release();
	}
	if (m_ReadBackAvgLumBuffer)
	{
		m_ReadBackAvgLumBuffer->Release();
	}
	if (m_ReadBackMiddleAvgLumBuffer)
	{
		m_ReadBackMiddleAvgLumBuffer->Release();
	}
	if (m_Shadowmap)
	{
		m_Shadowmap->Release();
	}
	if (m_PlayerShadowmap)
	{
		m_PlayerShadowmap->Release();
	}
	if (m_RWHDRTex_1_16)
	{
		m_RWHDRTex_1_16->Release();
	}
	if (m_RWBloomTex)
	{
		m_RWBloomTex->Release();
	}
	if (m_RWMiddleBloomTex)
	{
		m_RWMiddleBloomTex->Release();
	}

	if (m_ShadowmapHeap)
	{
		m_ShadowmapHeap->ReleaseObjects();
		delete m_ShadowmapHeap;
		m_ShadowmapHeap = nullptr;
	}

	if (m_verticalShader)
	{
		m_verticalShader->ReleaseObjects();
		delete m_verticalShader;
		m_verticalShader = nullptr;
	}

	if (m_horizenShader)
	{
		m_horizenShader->ReleaseObjects();
		delete m_horizenShader;
		m_horizenShader = nullptr;
	}

	if (m_downScaleFirstPassShader)
	{
		m_downScaleFirstPassShader->ReleaseObjects();
		delete m_downScaleFirstPassShader;
		m_downScaleFirstPassShader = nullptr;
	}

	if (m_downScaleSecondPassShader)
	{
		m_downScaleSecondPassShader->ReleaseObjects();
		delete m_downScaleSecondPassShader;
		m_downScaleSecondPassShader = nullptr;
	}

	if (m_BloomRevealShader)
	{
		m_BloomRevealShader->ReleaseObjects();
		delete m_BloomRevealShader;
		m_BloomRevealShader = nullptr;
	}
	  
	CMaterial::ReleaseShaders(); 

	SkillStg::GetInstance()->ReleaseObjects();
	StaticObjectStorage::GetInstance()->ReleaseObjects();
	TextureStorage::GetInstance()->ReleaseObjects();
	ModelStorage::GetInstance()->ReleaseObjects();
	ShaderManager::GetInstance()->ReleaseObjects();
	HitEffectMgr::GetInstance()->ReleaseObjects();
	SceneMgr::GetInstance()->ReleaseObjects(); 
	
	SkillStg::ReleaseInstance();
	StaticObjectStorage::ReleaseInstance();
	ShaderManager::ReleaseInstance();
	TextureStorage::ReleaseInstance();
	ModelStorage::ReleaseInstance();
	HitEffectMgr::ReleaseInstance();
	SceneMgr::ReleaseInstance();
	BossSkillMgr::ReleaseInstance();
	SoundManager::ReleaseInstance();

}

void GameFramework::UpdateGamelogic(float fElapsedTime)
{
	GameInput::Update(m_hWnd);
	Scene* pCurrScene = SceneMgr::GetInstance()->GetCurrScene();
	if (pCurrScene)
	{
		pCurrScene->ProcessInput(m_hWnd, fElapsedTime);
		pCurrScene->UpdatePhysics(fElapsedTime);
		pCurrScene->Update(fElapsedTime);
		pCurrScene->AnimateObjects(fElapsedTime);
		pCurrScene->LastUpdate(fElapsedTime);
	}
	GameInput::Reset();
}

void GameFramework::WaitForGpuComplete()
{
	const UINT64 nFenceValue = ++m_nFenceValues[m_SwapChainBufferIndex];
	HRESULT hResult = m_CommandQueue->Signal(m_Fence.Get(), nFenceValue);

	if (m_Fence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_Fence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void GameFramework::MoveToNextFrame()
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

void GameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	Scene* pCurrScene = SceneMgr::GetInstance()->GetCurrScene();
	if(pCurrScene)	pCurrScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	 
	switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL: 
		break;
	case WM_LBUTTONDOWN: 
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP: 
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:  
		break;

	default:
		break;
	}
}

static BOOL is_fullscreen = FALSE;
void GameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	Scene* pCurrScene = SceneMgr::GetInstance()->GetCurrScene();
	bool isWinScene = SceneMgr::GetInstance()->IsWinScene();
	bool isLoseScene = SceneMgr::GetInstance()->IsLoseScene();
	if(pCurrScene) pCurrScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam, 0.f);
	
	if (isWinScene || isLoseScene) return;

	bool isStateChange = false;
	switch (nMessageID)
	{
	case WM_KEYDOWN:  

		if (wParam != VK_F1)
		{ 
			// 만약 아무키나 누르면 장면 전환 ///////////////////
			if (SceneMgr::GetInstance()->IsMainScene())
			{
				SceneMgr::GetInstance()->ChangeSceneToSkillSelect();
			}
		}

		switch (wParam)
		{

		case MYVK_U: // 블룸과 톤 커브 모두 비활성화u
		{
			PostprocessingSetting::DeactiveBloom();
			PostprocessingSetting::DeactiveTonecurve();
			break;
		}

		case MYVK_I: // 블룸만 활성화
		{
			PostprocessingSetting::ActiveBloom();
			PostprocessingSetting::DeactiveTonecurve();
			break;
		}

		case MYVK_O: // 톤커브만 활성화
		{
			PostprocessingSetting::DeactiveBloom();
			PostprocessingSetting::ActiveTonecurve();
			break;
		}

		case MYVK_P: // 톤커브와 블룸 활성화
		{
			PostprocessingSetting::ActiveBloom();
			PostprocessingSetting::ActiveTonecurve();
			break;
		}
		 
		case VK_F5: // Main 으로 전환
		{
			isStateChange = true;
			SceneMgr::GetInstance()->ChangeSceneToMain();
			break;
		}
		case VK_F6: // Skill Select Scene 으로 전환
		{
			isStateChange = true;
			SceneMgr::GetInstance()->ChangeSceneToSkillSelect();
			break;
		}
		case VK_F7: // Game Scene 으로 전환
		{
			isStateChange = true;
			SceneMgr::GetInstance()->ChangeSceneToGame();
			break;
		}
		case VK_F8: // Win Scene 으로 전환
		{
			isStateChange = true;
			SceneMgr::GetInstance()->ChangeSceneToWin();
			break;
		}
		case VK_F9: // Lose Scene 으로 전환
		{
			isStateChange = true;
			SceneMgr::GetInstance()->ChangeSceneToLose();
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
		case MYVK_U:
		case MYVK_I:
		case MYVK_O:
		case MYVK_P:
		case VK_F5:
		case VK_F6:  
		case VK_F7:  
		case VK_F8:  
		case VK_F9:  
		{ 
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}
	 
}
 
void GameFramework::DefferedRenderOnSwapchain()
{
	RenderOnRT(&GameFramework::DefferedRenderSwapChain, m_RenderTargetBuffers[m_SwapChainBufferIndex], m_SwapChainCPUHandle[m_SwapChainBufferIndex], m_DepthStencilCPUHandle);
}

void GameFramework::RenderShadowMap()
{
	////파이프라인 상태를 설정한다.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);

	float anotherWidth = static_cast<float>(GameScreen::GetClientWidth()) / 4;
	float anotherHeight = static_cast<float>(GameScreen::GetClientHeight()) / 4;

	float width = static_cast<float>(GameScreen::GetClientWidth());
	float height = static_cast<float>(GameScreen::GetClientHeight());
	 
	m_ShadowmapHeap->UpdateShaderVariable(m_CommandList.Get());

	// 장면을 렌더합니다. 
	auto handle = m_ShadowmapHeap->GetGPUDescriptorHandleForHeapStart();

	// 장면을 렌더합니다.
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

void GameFramework::BuildShaders()
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
 
void GameFramework::RenderOnRT(renderFuncPtr renderPtr, ID3D12Resource* pRendertargetResource, D3D12_CPU_DESCRIPTOR_HANDLE hCPUrenderTarget, D3D12_CPU_DESCRIPTOR_HANDLE hCPUdepthStencil)
{
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), pRendertargetResource, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_CommandList->ClearRenderTargetView(hCPUrenderTarget, m_clearValueColor, 0, NULL);
	m_CommandList->ClearDepthStencilView(hCPUdepthStencil, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	m_CommandList->OMSetRenderTargets(1, &hCPUrenderTarget, TRUE, &hCPUdepthStencil);
	
	(this->*renderPtr)(); 
	 
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), pRendertargetResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void GameFramework::RenderOnRTs(renderFuncPtr renderPtr, UINT RenderTargetCount, ID3D12Resource ** pRendertargetResources, D3D12_CPU_DESCRIPTOR_HANDLE * hCPUrenderTargets, D3D12_CPU_DESCRIPTOR_HANDLE hCPUdepthStencils)
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

void GameFramework::RenderOnGbuffer()
{
	Scene* pCurrScene = SceneMgr::GetInstance()->GetCurrScene();
	if (pCurrScene)
	{
		pCurrScene->Render(m_CommandList.Get(), true);
	} 
}
 
LRESULT GameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			CGameTimer::GetInstance()->Stop();
			GameInput::Stop(hWnd);
		}
		else
		{
			CGameTimer::GetInstance()->Start();
			GameInput::Start(hWnd); 
		}
		break;
	}

	case WM_SIZE:
	{
		GameScreen::SetScreen(LOWORD(lParam) , HIWORD(lParam));

		//윈도우의 크기가 변경되면 후면버퍼의 크기를 변경한다.
		OnResizeBackBuffers();
		break;
	}

	case WM_MOUSEMOVE: 
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
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

void GameFramework::OnResizeBackBuffers()
{
	WaitForGpuComplete();

	m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	
	// 순서변경X ////////////
	ReleaseSwapChainBuffer();
	ReleaseGBuffers();
	ReleaseDepthStencilBuffer(); 
	// 순서변경X ////////////


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

	// 순서변경X ////////////
	CreateRenderTargetView();
	CreateGBufferView();
	CreateDepthStencilView();  
	// 순서변경X ////////////
	

	m_CommandList->Close();
	
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();
}

void GameFramework::Blur()
{
	// MiddleBloom 텍스쳐를 블러링하여 Bloom 텍스쳐로 저장하는 PASS 입니다. //////////////////////////////////

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWBloomTex, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// 수평 블러 //////////////////////////////////
	m_horizenShader->SetPSO(m_CommandList.Get()); 

	// 사용할 리소스 업데이트
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForMiddleBloom));
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(2));
	// 사용할 리소스 업데이트

	UINT groupX = (UINT)ceilf(float(GameScreen::GetWidth()) / 256.F / 4.f);
	m_CommandList->Dispatch(groupX, GameScreen::GetHeight() / 4.f, 1);
	// 수평 블러 //////////////////////////////////


	// 수직 블러 ////////////////////////////////// 
	m_verticalShader->SetPSO(m_CommandList.Get()); 

	// 사용할 리소스 업데이트
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForMiddleBloom));
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(2));
	// 사용할 리소스 업데이트

	UINT groupY = (UINT)ceilf(GameScreen::GetHeight() / 256.F / 4.f);
	m_CommandList->Dispatch(GameScreen::GetWidth() / 4.f, groupY, 1);
	// 수직 블러 //////////////////////////////////

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWBloomTex, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON);
}

void GameFramework::Bloom()
{   
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWHDRTex_1_16, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// Bloom 을 위해 휘도값을 MiddleBloom 텍스쳐에 저장하는 PASS //////////////////////////////////
	m_BloomRevealShader->SetPSO(m_CommandList.Get());

	// 사용할 리소스 업데이트
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForHDR1_16));
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(1));
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());
	// 사용할 리소스 업데이트

	UINT groupX = (UINT)ceil((float)((GameScreen::GetWidth() * GameScreen::GetHeight()) / 16.f) / 1024.f);
	m_CommandList->Dispatch(groupX, 1, 1);
	// Bloom 을 위해 휘도값을 MiddleBloom 텍스쳐에 저장하는 PASS //////////////////////////////////
	  
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RWHDRTex_1_16, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON);
}

void GameFramework::DownScale()
{  
	// 다운 스케일 첫번째 PASS //////////////////////////////////
	m_downScaleFirstPassShader->SetPSO(m_CommandList.Get());

	// 사용할 리소스 업데이트
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUDescriptorHandleForHeapStart());
	m_CommandList->SetComputeRootDescriptorTable(ROOTPARAMETER_UAV, m_GBufferHeap->GetGPUUAVDescriptorHandle(0));
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());
	// 사용할 리소스 업데이트

	UINT groupX = (UINT)ceil((float)((GameScreen::GetWidth() * GameScreen::GetHeight()) / 16.f) / 1024.f);
	m_CommandList->Dispatch(groupX, 1, 1);
    // 다운 스케일 첫번째 PASS //////////////////////////////////


	// 다운 스케일 두번째 PASS //////////////////////////////////
	m_downScaleSecondPassShader->SetPSO(m_CommandList.Get());

	// 사용할 리소스 업데이트 
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetComputeRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());
	// 사용할 리소스 업데이트
	 
	m_CommandList->Dispatch(1, 1, 1);
	// 다운 스케일 두번째 PASS //////////////////////////////////


	// 리소스 카피 
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWMiddleAvgLum, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));
	m_CommandList->CopyResource(m_ReadBackMiddleAvgLumBuffer, m_RWMiddleAvgLum);
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWMiddleAvgLum, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWAvgLum, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));
	m_CommandList->CopyResource(m_ReadBackAvgLumBuffer, m_RWAvgLum);
	m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RWAvgLum, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
	// 리소스 카피  

}
 
void GameFramework::RenderSwapChain()
{  
	// 장면을 렌더합니다.
	Scene* pCurrScene = SceneMgr::GetInstance()->GetCurrScene();
	if (pCurrScene)
	{ 
		pCurrScene->Render(m_CommandList.Get(), false);
	}

}

void GameFramework::DefferedRenderSwapChain()
{ 
	//////파이프라인 상태를 설정한다.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_DEFFREDRENDER, false);
	
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());
	 
	MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(m_CommandList.Get());
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(m_CommandList.Get(), ROOTPARAMETER_CAMERA);
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateLightShaderVariables(m_CommandList.Get(), &LightManager::m_pLights->m_pLights[2]);

	//// 리소스만 바꾼다.. 
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
	m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_SHADOWTEXTURE, handle); // 마지막은 쉐도우맵
	
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);
	 
}

void GameFramework::RenderToTexture()
{
	//////파이프라인 상태를 설정한다.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);

	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

	MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(m_CommandList.Get());
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(m_CommandList.Get(), ROOTPARAMETER_CAMERA);
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateLightShaderVariables(m_CommandList.Get(), &LightManager::m_pLights->m_pLights[2]);

	//// 리소스만 바꾼다.. 
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_GBufferHeap->GetGPUDescriptorHandleForHeapStart(); // 0번째 리소스
	handle.ptr = handle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize * (m_GBufferForBloom);

	D3D12_VIEWPORT	Viewport{ 0.f, 0.f, GameScreen::GetWidth() , GameScreen::GetHeight(), 1.0f, 0.0f };
	D3D12_RECT		ScissorRect{ 0, 0, GameScreen::GetWidth() , GameScreen::GetHeight() };

	m_CommandList->RSSetViewports(1, &Viewport);
	m_CommandList->RSSetScissorRects(1, &ScissorRect);
	 
	m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, handle); 
	
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);
}

void GameFramework::RenderForShadow()
{
	Scene* pCurrScene = SceneMgr::GetInstance()->GetCurrScene();
	if (pCurrScene)
	{
		pCurrScene->RenderForShadow(m_CommandList.Get());
	} 
}

void GameFramework::RenderForPlayerShadow()
{
	Scene* pCurrScene = SceneMgr::GetInstance()->GetCurrScene();
	if (pCurrScene)
	{
		pCurrScene->RenderForPlayerShadow(m_CommandList.Get());
	}
}

void GameFramework::ToneCurveAndBloom()
{ 
	//////파이프라인 상태를 설정한다.
	if (PostprocessingSetting::IsBloom() && PostprocessingSetting::IsToneCurve())
	{
		ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_TONECURVEANDBLOOM, false);
	}
	else if (!PostprocessingSetting::IsBloom() && PostprocessingSetting::IsToneCurve())
	{
		ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_TONECURVE, false);
	}
	else if ( PostprocessingSetting::IsBloom() && !PostprocessingSetting::IsToneCurve())
	{
		ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_BLOOM, false);
	}
	else
	{
		ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);
	}

	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

	if (MainCameraMgr::GetMainCamera())
	{
		MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(m_CommandList.Get());
		MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(m_CommandList.Get(), ROOTPARAMETER_CAMERA);
		MainCameraMgr::GetMainCamera()->GetCamera()->UpdateLightShaderVariables(m_CommandList.Get(), &LightManager::m_pLights->m_pLights[2]);
	}

	m_CommandList->SetGraphicsRootUnorderedAccessView(ROOTPARAMETER_MIDDLEAVGLUM, m_RWMiddleAvgLum->GetGPUVirtualAddress());
	m_CommandList->SetGraphicsRootUnorderedAccessView(ROOTPARAMETER_AVGLUM, m_RWAvgLum->GetGPUVirtualAddress());

	//// 리소스만 바꾼다.. 

	D3D12_VIEWPORT	Viewport{ 0.f, 0.f, GameScreen::GetWidth() , GameScreen::GetHeight(), 1.0f, 0.0f };
	D3D12_RECT		ScissorRect{ 0, 0, GameScreen::GetWidth() , GameScreen::GetHeight() };

	m_CommandList->RSSetViewports(1, &Viewport);
	m_CommandList->RSSetScissorRects(1, &ScissorRect);

	if (!PostprocessingSetting::IsBloom() && !PostprocessingSetting::IsToneCurve())
	{
		m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, m_GBufferHeap->GetGPUSrvDescriptorHandle(0));
	}
	else
	{ 
		m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTURE, m_GBufferHeap->GetGPUSrvDescriptorHandle(0));
		m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, m_GBufferHeap->GetGPUSrvDescriptorHandle(m_GBufferForBloom));
	}

	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);

}
