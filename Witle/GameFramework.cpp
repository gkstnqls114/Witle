#include "stdafx.h"
#include "d3dx12.h"
#include "d3dUtil.h"

//// GameBase ////////////////////////// 
#include "GameTimer.h"
#include "GameInput.h"
#include "GameScreen.h" 
#include "MainCameraMgr.h"
//// GameBase ////////////////////////// 

//// Manager ////////////////////////// 
#include "GraphicsRootSignatureMgr.h"
#include "ShaderManager.h"
#include "ModelStorage.h"
#include "TextureStorage.h"
#include "StaticObjectStorage.h"
#include "SceneMgr.h"
//// Manager ////////////////////////// 

//// Scene //////////////////////////  
#include "GameScene.h"
//// Scene ////////////////////////// 
 
#include "QuadtreeTerrain.h"
#include "CameraObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Object.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
 
#include "GameFramework.h"

static const bool DefferedRendering = false;

void CGameFramework::Render()
{
	HRESULT hResult = m_CommandAllocator->Reset();
	hResult = m_CommandList->Reset(m_CommandAllocator.Get(), NULL);

	if (DefferedRendering)
	{
		RenderForShadow();
		//// GBuffer에 Render //////////////////////////
		RenderOnGbuffer();
		//// ComputeShader ////////////////////////// 
		//// ComputeShader ////////////////////////// 
		DefferedRenderOnSwapchain();
	}
	else
	{
		RenderForShadow();
		//// SwapChain에 Render //////////////////////////
		RenderOnSwapchain();
	}

	//// SwapChain에 Render //////////////////////////


	hResult = m_CommandList->Close();
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	m_SwapChain->Present(0, 0);

	MoveToNextFrame();
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

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	// CreateRWResourceViews();
	CreateShadowmapView();

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

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->DrawInstanced(6, 1, 0, 0);

		handle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}
}


void CGameFramework::CreateRWResourceViews()
{
	D3D12_CLEAR_VALUE d3dClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM,
	{m_RWClearValue[0], m_RWClearValue[1], m_RWClearValue[2], m_RWClearValue[3] } };

	// 리소스 생성
	// 버퍼도 아니고, 리소스 상태가 깊이 스텐실도 렌더 타겟도 아니라면 클리어밸류는 NULL
	m_ComputeRWResource = d3dUtil::CreateTexture2DResource
		(m_d3dDevice.Get(), m_CommandList.Get(), 
			GameScreen::GetWidth(), GameScreen::GetHeight(),
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*다름*/,
			D3D12_RESOURCE_STATE_COMMON/*다름*/,
			NULL/*, 0*/ /*인덱스*/); 
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

void CGameFramework::CreateDirect3DDevice()
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

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
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

void CGameFramework::CreateShadowmapView()
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
	//// 리소스 생성 /////////////////////////////////////////////////

	// 디스크립터 생성 /////////////////////////////////////////////
	m_ShadowmapHeap = new MyDescriptorHeap();
	m_ShadowmapHeap->CreateCbvSrvUavDescriptorHeaps(m_d3dDevice.Get(), m_CommandList.Get(), 0, 1, 0);
	m_hCpuSrvForShadow = m_ShadowmapHeap->GetCPUDescriptorHandleForHeapStart(); // 쉐이더 리소스 뷰

	 // Create SRV to resource so we can sample the shadow map in a shader program.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice = 0;
	m_d3dDevice->CreateShaderResourceView(m_Shadowmap, &srvDesc, m_hCpuSrvForShadow);

	m_hCpuDsvForShadow = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_hCpuDsvForShadow.ptr += d3dUtil::gnDsvDescriptorIncrementSize;
	m_hCpuDsvForShadow.ptr += d3dUtil::gnDsvDescriptorIncrementSize;

	// Create DSV to resource so we can render to the shadow map.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.Texture2D.MipSlice = 0;
	m_d3dDevice->CreateDepthStencilView(m_Shadowmap, &dsvDesc, m_hCpuDsvForShadow);

	m_ShadowmapCPUHandle = m_hCpuDsvForShadow;
	// 디스크립터 생성 /////////////////////////////////////////////
	  
	
	//DXGI_FORMAT textureType = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	//D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	// 
	//// 해당 텍스쳐를 2디 텍스쳐로 만들어서 담는다.
	//m_Shadowmap =
	//	d3dUtil::CreateTexture2DResource(
	//		m_d3dDevice.Get(), m_CommandList.Get(),
	//		GameScreen::GetClientWidth(), GameScreen::GetClientHeight(),
	//		textureType, 
	//		flag,
	//		D3D12_RESOURCE_STATE_DEPTH_WRITE, NULL
	//);
	//  
	//// 렌더 타겟 뷰는 생성하지 않음

	//// 쉐이더 리소스 뷰 생성
	//if (!m_ShadowmapHeap)
	//{
	//	m_ShadowmapHeap = new MyDescriptorHeap;
	//	m_ShadowmapHeap->CreateCbvSrvUavDescriptorHeaps(m_d3dDevice.Get(), m_CommandList.Get(), 0, m_ShadowmapCount, 0);
	//	for (int i = 0; i < m_ShadowmapCount; ++i)
	//	{
	//		m_ShadowmapHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_CommandList.Get(), 
	//			 m_Shadowmap, RESOURCE_TEXTURE2D, i, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
	//	}
	//}


	//// Shadow Map ///////////////////////////////////////////////
	//// 마찬가지로 쉐도우 맵은 깊이 스텐실 뷰

	//D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDescForShadow;
	//d3dDepthStencilViewDescForShadow.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // R16이 아니라 D16!
	//d3dDepthStencilViewDescForShadow.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//d3dDepthStencilViewDescForShadow.Texture2D.MipSlice = 0;
	//d3dDepthStencilViewDescForShadow.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;

	//ResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;


	//D3D12_CLEAR_VALUE ClearValueForShadow;
	//ClearValueForShadow.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//ClearValueForShadow.DepthStencil.Depth = 1.0f;
	//ClearValueForShadow.DepthStencil.Stencil = 0;

	//m_ShadowmapCPUHandle = m_DepthStencilCPUHandle;
	//m_ShadowmapCPUHandle.ptr += m_DsvDescriptorSize; // 3번째 깊이 스텐실 뷰.
	//m_ShadowmapCPUHandle.ptr += m_DsvDescriptorSize; // 3번째 깊이 스텐실 뷰.

	//// Shadow map 깊이 리소스 자원 생성
	//m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
	//	&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_READ, &ClearValueForShadow, IID_PPV_ARGS(&m_Shadowmap));

	//m_d3dDevice->CreateDepthStencilView(m_Shadowmap, &d3dDepthStencilViewDescForShadow, m_ShadowmapCPUHandle);

	//// 힙에 추가
	//m_ShadowmapHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_CommandList.Get(), m_Shadowmap, RESOURCE_TEXTURE2D,
	//	0, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

}

void CGameFramework::CreateCommandQueueAndList()
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
	m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_CommandList.Get(), m_GBuffersForDepth[0], RESOURCE_TEXTURE2D, m_GBuffersCount - 1/*맨 마지막에 배치*/, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);


}

void CGameFramework::CreateGBufferView()
{ 
	for (UINT i = 0; i < m_GBuffersCountForRenderTarget; i++)
	{ 
		DXGI_FORMAT textureType ;
		D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		 
		if (i == 1) // 노말값
		{
			textureType = DXGI_FORMAT_R11G11B10_FLOAT; // 32BIT 노멀 

		}
		else
		{
			textureType = DXGI_FORMAT_R8G8B8A8_UNORM; 
			 
		}
		D3D12_CLEAR_VALUE d3dClearValue = { textureType,
		{m_GBufferClearValue[i][0], m_GBufferClearValue[i][1], m_GBufferClearValue[i][2], m_GBufferClearValue[i][3] } };

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
		 
		if (i == 1)
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
		m_GBufferHeap->CreateCbvSrvUavDescriptorHeaps(m_d3dDevice.Get(), m_CommandList.Get(), 0, m_GBuffersCount, 0);
		for (int i = 0; i < m_GBuffersCountForRenderTarget; ++i)
		{
			m_GBufferHeap->CreateShaderResourceViews(m_d3dDevice.Get(), m_CommandList.Get(), m_GBuffersCountForRenderTarget, m_GBuffersForRenderTarget[i], RESOURCE_TEXTURE2D, i);
		} 
	} 
}

void CGameFramework::BuildObjects()
{
	HRESULT hResult;

	m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	
	///////////////////////////////////////////////////////////////////////////// 리소스 생성
	// 루트 시그니처 먼저 생성
	GraphicsRootSignatureMgr::BuildObject(m_d3dDevice.Get());

	m_SceneMgr = new SceneMgr; 

	// 순서 변경 X /////////////
	// 루트 시그니처를 통해 모든 오브젝트 갖고온다.
	TextureStorage::GetInstance()->CreateTextures(m_d3dDevice.Get(), m_CommandList.Get());
	ModelStorage::GetInstance()->CreateModels(m_d3dDevice.Get(), m_CommandList.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	   
	BuildShaders();
	 
	m_SceneMgr->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get());

	// 터레인을 위한 쉐도우 맵과 텍스쳐 연결하는 디스크립터 힙 생성...
	GameScene::CreateShaderResourceViewsForShadow(m_d3dDevice.Get(), m_Shadowmap, ROOTPARAMETER_SHADOWTEXTURE, false, 3); 
	// 순서 변경 X /////////////


	///////////////////////////////////////////////////////////////////////////// 리소스 생성

	hResult = m_CommandList->Close();
	assert(hResult == S_OK);
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();

	///////////////////////////////////////////////////////////////////////////// 업로드 힙 릴리즈
	if (m_SceneMgr) m_SceneMgr->ReleaseUploadBuffers();
	 
	TextureStorage::GetInstance()->ReleaseUploadBuffers();
	ModelStorage::GetInstance()->ReleaseUploadBuffers();
	///////////////////////////////////////////////////////////////////////////// 업로드 힙 릴리즈

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
	 

	CMaterial::ReleaseShaders(); 
	StaticObjectStorage::GetInstance()->ReleaseObjects();
	TextureStorage::GetInstance()->ReleaseObjects();
	ModelStorage::GetInstance()->ReleaseObjects();
	ShaderManager::GetInstance()->ReleaseObjects();

	StaticObjectStorage::ReleaseInstance();
	ShaderManager::ReleaseInstance();
	TextureStorage::ReleaseInstance();
	ModelStorage::ReleaseInstance();
	 
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
		//마우스 캡쳐를 하고 현재 마우스 위치를 가져온다. 
		GameInput::SetCapture(hWnd);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		//마우스 캡쳐를 해제한다.
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
	if (m_SceneMgr) {
		m_SceneMgr->GetCurrScene()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam, 0.f);
	}

	switch (nMessageID)
	{
	case WM_KEYDOWN:   
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
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CGameFramework::RenderOnSwapchain()
{
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RenderTargetBuffers[m_SwapChainBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_CommandList->ClearRenderTargetView(m_SwapChainCPUHandle[m_SwapChainBufferIndex], /*pfClearColor*/Colors::Gray, 0, NULL);
	m_CommandList->ClearDepthStencilView(m_DepthStencilCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	m_CommandList->OMSetRenderTargets(1, &m_SwapChainCPUHandle[m_SwapChainBufferIndex], TRUE, &m_DepthStencilCPUHandle);

	////그래픽 루트 시그너쳐를 설정한다.
	m_CommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	
	// 기본 게임 장면을 렌더한다.
	RenderSwapChain();

	// 하단에 그림자 맵을 렌더링한다.
	RenderShadowMap();

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RenderTargetBuffers[m_SwapChainBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void CGameFramework::DefferedRenderOnSwapchain()
{
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RenderTargetBuffers[m_SwapChainBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_CommandList->ClearRenderTargetView(m_SwapChainCPUHandle[m_SwapChainBufferIndex], /*pfClearColor*/Colors::Gray, 0, NULL);
	m_CommandList->ClearDepthStencilView(m_DepthStencilCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	// 기본 게임 장면을 렌더한다.
	DefferedRenderSwapChain();

	// 하단에 테스트용으로 보일 리소스들을 렌더한다.
	RenderGBuffers();

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RenderTargetBuffers[m_SwapChainBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	
}

void CGameFramework::RenderShadowMap()
{
	////파이프라인 상태를 설정한다.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_SHOWTEXTURE, false);

	float anotherWidth = static_cast<float>(GameScreen::GetClientWidth()) / 4;
	float anotherHeight = static_cast<float>(GameScreen::GetClientHeight()) / 4;

	float width = static_cast<float>(GameScreen::GetClientWidth());
	float height = static_cast<float>(GameScreen::GetClientHeight());


	m_ShadowmapHeap->UpdateShaderVariable(m_CommandList.Get());

	// 장면을 렌더합니다. 
	D3D12_VIEWPORT	GBuffer_Viewport{ 0, height - anotherHeight, anotherWidth , anotherHeight, 1.0f, 0.0f };
	D3D12_RECT		ScissorRect{ 0, height - anotherHeight, anotherWidth * (3 ) , height + anotherHeight };

	m_CommandList->RSSetViewports(1, &GBuffer_Viewport);
	m_CommandList->RSSetScissorRects(1, &ScissorRect);

	m_CommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_TEXTUREBASE, m_ShadowmapHeap->GetGPUDescriptorHandleForHeapStart());

	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);
}

void CGameFramework::BuildShaders()
{
	CMaterial::PrepareShaders(m_d3dDevice.Get(), m_CommandList.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	ShaderManager::GetInstance()->BuildShaders(m_d3dDevice.Get(), GraphicsRootSignatureMgr::GetGraphicsRootSignature());
}
 
void CGameFramework::RenderOnGbuffer()
{
	for (int i = 0; i < m_GBuffersCountForRenderTarget; i++)
	{ 
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_GBuffersForRenderTarget[i], D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ClearRenderTargetView(m_GBufferCPUHandleForRenderTarget[i], m_GBufferClearValue[i], 0, NULL);
	}  

	m_CommandList->ClearDepthStencilView(m_GBufferCPUHandleForDepth[0], D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	m_CommandList->OMSetRenderTargets(m_GBuffersCountForRenderTarget, m_GBufferCPUHandleForRenderTarget, TRUE, &m_GBufferCPUHandleForDepth[0]);
	
	if (m_SceneMgr)
	{
		m_SceneMgr->GetCurrScene()->Render(m_CommandList.Get(), true);
	}

	for (int x = 0; x < m_GBuffersCountForRenderTarget; ++x)
	{
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_GBuffersForRenderTarget[x], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	}
}

void CGameFramework::RenderOnCompute()
{
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

		//윈도우의 크기가 변경되면 후면버퍼의 크기를 변경한다.
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

void CGameFramework::Blur()
{

}

void CGameFramework::RenderSwapChain()
{  
	// 장면을 렌더합니다.
	if (m_SceneMgr->GetCurrScene())
	{ 
		m_SceneMgr->GetCurrScene()->Render(m_CommandList.Get(), false);
	}
}

void CGameFramework::DefferedRenderSwapChain()
{
	m_CommandList->OMSetRenderTargets(1, &m_SwapChainCPUHandle[m_SwapChainBufferIndex], TRUE, &m_DepthStencilCPUHandle);

	//////그래픽 루트 시그너쳐를 설정한다.
	m_CommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	//////파이프라인 상태를 설정한다.
	ShaderManager::GetInstance()->SetPSO(m_CommandList.Get(), SHADER_DEFFREDRENDER, false);
	
	m_GBufferHeap->UpdateShaderVariable(m_CommandList.Get());

	MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(m_CommandList.Get());
	MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(m_CommandList.Get(), ROOTPARAMETER_CAMERA);

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
	
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->DrawInstanced(6, 1, 0, 0);
	 
}

void CGameFramework::RenderForShadow()
{  
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	 
	m_CommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	// SHADOW 설정
	m_CommandList->OMSetRenderTargets(0, NULL, TRUE, &m_ShadowmapCPUHandle);
	m_CommandList->ClearDepthStencilView(m_ShadowmapCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	////그래픽 루트 시그너쳐를 설정한다.

	m_SceneMgr->GetCurrScene()->RenderForShadow(m_CommandList.Get());
	// m_pScene->Render(m_CommandList.Get(), false);

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_Shadowmap, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
}
