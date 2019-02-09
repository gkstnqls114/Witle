#include "stdafx.h"
#include "ShaderManager.h"
#include "GameTimer.h"
#include "GameInput.h"
#include "d3dUtil.h"
#include "CubeShader.h"
#include "TerrainShader.h"
#include "GameScreen.h" 
#include "GameScene.h"

#include "GameFramework.h"


void CGameFramework::Render()
{
	HRESULT hResult = m_CommandAllocator->Reset();
	hResult = m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	 
	RenderSwapChain();

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

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	//CreateGBufferView();
	
	//CreateComputePipelineState();
	// CreateRWResourceViews();

	//CreateMRTComputeShader();
	 
	BuildObjects();
	 

	return true;
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	::CloseHandle(m_hFenceEvent);

}

D3D12_SHADER_BYTECODE CGameFramework::CreateComputeShader(ID3DBlob ** ppd3dShaderBlob, LPCSTR pszShaderName)
{
	//  Shader::CompileShaderFromFile 와 같다.

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	::D3DCompileFromFile(L"ComputeShader.hlsl", NULL, NULL, pszShaderName, "cs_5_1", nCompileFlags, 0, ppd3dShaderBlob, NULL);

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

	return(d3dShaderByteCode); 
}

//
//ID3D12RootSignature* CGameFramework::CreateComputeRootSignature()
//{
//	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[2];
//
//	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
//	pd3dDescriptorRanges[0].NumDescriptors = 4;
//	pd3dDescriptorRanges[0].BaseShaderRegister = 0; // t0 , t1 , t2, t3
//	pd3dDescriptorRanges[0].RegisterSpace = 0;
//	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;
//
//	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
//	pd3dDescriptorRanges[1].NumDescriptors = 1;
//	pd3dDescriptorRanges[1].BaseShaderRegister = 0; // u0
//	pd3dDescriptorRanges[1].RegisterSpace = 0;
//	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = 0;
//
//	D3D12_ROOT_PARAMETER pd3dRootParameters[2];
//
//	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
//	pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0];
//	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
//	// Texture2D gtxtTexture : register(t0);
//	// Texture2D gtxtTextureGreen : register(t1);
//	// Texture2D gtxtTextureBlue : register(t2);
//
//	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
//	pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; // gtxtRWOutput (s0)
//	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
//
//	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
//	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
//	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
//	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	d3dSamplerDesc.MipLODBias = 0;
//	d3dSamplerDesc.MaxAnisotropy = 1;
//	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
//	d3dSamplerDesc.MinLOD = 0;
//	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
//	d3dSamplerDesc.ShaderRegister = 0;
//	d3dSamplerDesc.RegisterSpace = 0;
//	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
//
//	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
//	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
//	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
//	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
//	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
//	d3dRootSignatureDesc.NumStaticSamplers = 1;
//	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
//	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;
//
//	ID3DBlob *pd3dSignatureBlob = NULL;
//	ID3DBlob *pd3dErrorBlob = NULL;
//	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
//	m_d3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_ComputeRootSignature);
//	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
//	if (pd3dErrorBlob) pd3dErrorBlob->Release();
//
//	return m_ComputeRootSignature.Get();
//}

//void CGameFramework::CreateComputePipelineState()
//{
//	ID3DBlob *pd3dComputeShaderBlob = nullptr;
//
//	D3D12_COMPUTE_PIPELINE_STATE_DESC computepipelinestateDESC = {};
//
//	// computepipelinestateDESC.CachedPSO; ?? 뭐하는 애야...
//
//	computepipelinestateDESC.pRootSignature = CreateComputeRootSignature();
//	computepipelinestateDESC.CS = CreateComputeShader(&pd3dComputeShaderBlob, "HorzBlurCS");
//	computepipelinestateDESC.Flags = D3D12_PIPELINE_STATE_FLAG_NONE; // D3D12_PIPELINE_STATE_TOOL_DEBUG 도 있당... 우왕...
//	computepipelinestateDESC.NodeMask = 0; // 단일 gpu : 0
//
//	m_d3dDevice->CreateComputePipelineState(&computepipelinestateDESC, IID_PPV_ARGS(&m_HorzComputePipelineState));
//
//	if (pd3dComputeShaderBlob) pd3dComputeShaderBlob->Release();
//
//
//	pd3dComputeShaderBlob = nullptr;
//
//	computepipelinestateDESC.pRootSignature = CreateComputeRootSignature();
//	computepipelinestateDESC.CS = CreateComputeShader(&pd3dComputeShaderBlob, "VertBlurCS");
//	computepipelinestateDESC.Flags = D3D12_PIPELINE_STATE_FLAG_NONE; 
//	computepipelinestateDESC.NodeMask = 0; // 단일 gpu : 0
//
//	m_d3dDevice->CreateComputePipelineState(&computepipelinestateDESC, IID_PPV_ARGS(&m_VertComputePipelineState));
//
//	if (pd3dComputeShaderBlob) pd3dComputeShaderBlob->Release();
//
//}
//
//void CGameFramework::CreateRWResourceViews()
//{
//	m_pComputeTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
//
//	D3D12_CLEAR_VALUE d3dClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM,
//	{m_RWClearValue[0], m_RWClearValue[1], m_RWClearValue[2], m_RWClearValue[3] } };
//
//	// 리소스 생성
//	// 버퍼도 아니고, 리소스 상태가 깊이 스텐실도 렌더 타겟도 아니라면 클리어밸류는 NULL
//	m_ComputeRWResource = m_pComputeTexture->CreateTexture
//		(m_d3dDevice.Get(), m_CommandList.Get(), 
//			FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT,
//			DXGI_FORMAT_R8G8B8A8_UNORM,
//			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*다름*/,
//			D3D12_RESOURCE_STATE_COMMON/*다름*/,
//			NULL, 0 /*인덱스*/);
//
//}
//
//void CGameFramework::CreateMRTComputeShader()
//{ 
//	m_RedShader.CreateGraphicsRootSignature(m_d3dDevice.Get());
//	m_RedShader.CreateShader(m_d3dDevice.Get(), m_RedShader.GetGraphicsRootSignature());
//	
//	m_GreenShader.CreateShader(m_d3dDevice.Get(), m_RedShader.GetGraphicsRootSignature());
//	m_BlueShader.CreateShader(m_d3dDevice.Get(), m_RedShader.GetGraphicsRootSignature());
//	m_RenderComputeShader.CreateShader(m_d3dDevice.Get(), m_RedShader.GetGraphicsRootSignature());
//
//	m_RedShader.CreateCbvAndSrvDescriptorHeaps(m_d3dDevice.Get(), m_CommandList.Get(), 0, m_GBuffersCount + 1, 1/*UAV 개수*/);
//	
//	m_RedShader.CreateShaderResourceViews(m_d3dDevice.Get(), m_CommandList.Get(), m_pGBufferTexture, G_BUFFER_ROOT_PARMATER, false);
//	m_RedShader.CreateShaderResourceViews(m_d3dDevice.Get(), m_CommandList.Get(), m_pComputeTexture, G_BUFFER_ROOT_PARMATER, false, m_GBuffersCount);
//
//	// 현재 구조상 일단 내부에서 뷰만 생성함... T _T SetArgument 설정 안하고있음
//	m_RedShader.CreateUnorderedAccessViews(m_d3dDevice.Get(), m_CommandList.Get(), m_pComputeTexture, COMPUTE_UAV_ROOT_PARAMETER, false);
//	m_UAVGPUDescriptorHandle = m_RedShader.GetGPUUAVDescriptorStartHandle();
//}

void CGameFramework::CreateSwapChain()
{
	m_nWndClientWidth = GameScreen::GetWidth();
	m_nWndClientHeight = GameScreen::GetHeight();

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	::ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = m_SwapChainBuffersCount;
	SwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	SwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
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
	OutputDebugString(L"...EnableDebugLayer\n");
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


	if (pAdapter) pAdapter->Release();
}

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	HRESULT hResult;

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = m_SwapChainBuffersCount + m_GBuffersCount;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // 렌더 타겟 뷰
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	//렌더 타겟 서술자 힙(서술자의 개수는 스왑체인 버퍼의 개수)을 생성한다.
	hResult = m_d3dDevice->CreateDescriptorHeap( &d3dDescriptorHeapDesc, IID_PPV_ARGS(&m_RtvHeap));
	assert(hResult == S_OK);

	//렌더 타겟 서술자 힙의 원소의 크기를 저장한다.
	m_RtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	//깊이-스텐실 서술자 힙(서술자의 개수는 1)을 생성한다.
	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // 깊이 스텐실 뷰 
	hResult = m_d3dDevice->CreateDescriptorHeap( &d3dDescriptorHeapDesc,  IID_PPV_ARGS(&m_DsvHeap)); assert(hResult == S_OK);

	//깊이-스텐실 서술자 힙의 원소의 크기를 저장한다.
	m_DsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
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

		m_d3dDevice->CreateRenderTargetView(m_RenderTargetBuffers[i].Get(), &d3dRenderTargetViewDesc, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += m_RtvDescriptorSize;
	}
}

void CGameFramework::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.Alignment = 0;
	ResourceDesc.Width = m_nWndClientWidth;
	ResourceDesc.Height = m_nWndClientHeight;
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
	m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue, IID_PPV_ARGS(&m_DepthStencilBuffer));

	//깊이-스텐실 버퍼를 생성한다.
	m_DepthStencilCPUHandle = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer.Get(), NULL, m_DepthStencilCPUHandle); 
}

//void CGameFramework::CreateGBufferView()
//{ 
//	m_pGBufferTexture = new CTexture(m_GBuffersCount, RESOURCE_TEXTURE2D_ARRAY, 0);
//
//	for (UINT i = 0; i < m_GBuffersCount; i++)
//	{
//		D3D12_CLEAR_VALUE d3dClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM,
//		{m_GBufferClearValue[i][0], m_GBufferClearValue[i][1], m_GBufferClearValue[i][2], m_GBufferClearValue[i][3] } };
//		
//		// 해당 텍스쳐를 2디 텍스쳐 어레이로 만들어서?? 버퍼에 담는다.
//		m_GBuffers[i] =
//			m_pGBufferTexture->CreateTexture(m_d3dDevice.Get(), m_CommandList.Get(), 
//				FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT,
//				DXGI_FORMAT_R8G8B8A8_UNORM,
//				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, 
//				D3D12_RESOURCE_STATE_GENERIC_READ, &d3dClearValue, i);
//
//		// m_GBuffers[i]->AddRef();
//	}
//
//	// 디스크립터 핸들 생성
//	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
//	d3dRtvCPUDescriptorHandle.ptr += (m_SwapChainBuffersCount * m_RtvDescriptorSize);
//
//	// 렌더타겟 뷰 생성
//	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
//	d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
//	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
//	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;
//
//	for (UINT i = 0; i < m_GBuffersCount; i++)
//	{
//		// 렌더타겟 뷰 생성
//		m_GBufferCPUHandle[i] = d3dRtvCPUDescriptorHandle;
//
//		// 디바이스에 렌더타겟뷰를 해당 텍스쳐로 설정된다...
//		m_d3dDevice->CreateRenderTargetView(m_pGBufferTexture->GetTexture(i), &d3dRenderTargetViewDesc, m_GBufferCPUHandle[i]);
//		d3dRtvCPUDescriptorHandle.ptr += m_RtvDescriptorSize;
//	}
//
//
//}

void CGameFramework::BuildObjects()
{
	HRESULT hResult;

	m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	
	///////////////////////////////////////////////////////////////////////////// 리소스 생성
	m_pScene = new GameScene;
	if (m_pScene) m_pScene->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get());
	
	Shader* pCubeShader = new CubeShader();
	pCubeShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("Cube", pCubeShader);

	Shader* pTerrainShader = new TerrainShader();
	pTerrainShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("Terrain", pTerrainShader); 
	///////////////////////////////////////////////////////////////////////////// 리소스 생성

	hResult = m_CommandList->Close();
	assert(hResult == S_OK);
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();

	///////////////////////////////////////////////////////////////////////////// 업로드 힙 릴리즈
	if (m_pScene) m_pScene->ReleaseUploadBuffers(); 
	///////////////////////////////////////////////////////////////////////////// 업로드 힙 릴리즈

	CGameTimer::GetInstance()->Reset();
}

void CGameFramework::ReleaseObjects()
{
	if (m_pScene) { 
		m_pScene->ReleaseObjects();
		delete m_pScene;
		m_pScene = nullptr;
	}
}

void CGameFramework::UpdateGamelogic(float fElapsedTime)
{
	if (m_pScene) {
		GameInput::Update(m_hWnd);
		m_pScene->ProcessInput(m_hWnd, fElapsedTime);
		m_pScene->Update(fElapsedTime);
		m_pScene->LastUpdate(fElapsedTime);

		// 현재 애니메이션 어디다가 쓰는지 모르겠다.
		//m_pScene->AnimateObjects(0);
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
	//if (m_pScene) {
	//	m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	//}

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
		break;

	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		
		case VK_F1:
		{
			BOOL bFullScreenState = FALSE;
			m_SwapChain->GetFullscreenState(&bFullScreenState, NULL);
			m_SwapChain->SetFullscreenState(!bFullScreenState, NULL);
			DXGI_MODE_DESC dxgiTargetParameters;
			dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dxgiTargetParameters.Width = m_nWndClientWidth;
			dxgiTargetParameters.Height = m_nWndClientHeight;
			dxgiTargetParameters.RefreshRate.Numerator = 60;
			dxgiTargetParameters.RefreshRate.Denominator = 1;
			dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			dxgiTargetParameters.ScanlineOrdering =
				DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			m_SwapChain->ResizeTarget(&dxgiTargetParameters);
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
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

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
		case 'Z':
		case 'z':
			d3dUtil::gTEST -= 1;
			std::cout << d3dUtil::gTEST << std::endl;
			break;

		case 'X':
		case 'x':
			d3dUtil::gTEST += 1;
			std::cout << d3dUtil::gTEST << std::endl;
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
	
	for (int i = 0; i < m_SwapChainBuffersCount; i++) {
		if (m_RenderTargetBuffers[i]) {
			m_RenderTargetBuffers[i].Reset();
		}
	}

	if (m_DepthStencilBuffer) {
		m_DepthStencilBuffer.Reset();
	}

#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	m_SwapChain->GetDesc(&SwapChainDesc);
	m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	m_SwapChainBufferIndex = 0;
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_SwapChain->GetDesc(&dxgiSwapChainDesc);
	m_SwapChain->ResizeBuffers(m_SwapChainBuffersCount, m_nWndClientWidth,
		m_nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
	m_SwapChainBufferIndex = 0;
#endif

	CreateRenderTargetView();
	CreateDepthStencilView();
	
	m_CommandList->Close();
	
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();
}

void CGameFramework::RenderGBuffers()
{

	////////////////////////////////////////////////////////// GBuffer 
	//for (int i = 0; i < m_GBuffersCount; i++) { 
	//	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_GBuffers[i], D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//}
	// 
	//for (int i = 0; i < m_GBuffersCount; i++) { 
	//	m_CommandList->ClearRenderTargetView(m_GBufferCPUHandle[i], m_GBufferClearValue[i], 0, NULL);
	//}
	// 
	//m_CommandList->ClearRenderTargetView(m_GBufferCPUHandle[m_SwapChainBufferIndex], m_GBufferClearValue[m_SwapChainBufferIndex], 0, NULL);
	//m_CommandList->ClearDepthStencilView(m_DepthStencilCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	//m_CommandList->OMSetRenderTargets(m_GBuffersCount, m_GBufferCPUHandle, TRUE, &m_DepthStencilCPUHandle);
	//
	////// 장면 렌더한다...
	////// 픽셀쉐이더에 참고하여 렌더타겟에 그린다...
	////// RenderTarget0 : 컬러
	////// RenderTarget1 : 노말
	////// RenderTarget2 : 오브젝트
	////// 실제로 그리는 것이 아니라... 정보를 저장한다...

	//// 장면을 렌더합니다.
	//if (m_pScene) {
	//	m_pScene->Render(m_CommandList.Get());
	//}
	// 
	//for (int i = 0; i < m_GBuffersCount; i++) { 
	//	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_GBuffers[i], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	//}
	////////////////////////////////////////////////////////// GBuffer
}

void CGameFramework::Blur()
{
	////////////////////////////////////////////////////////// 계산쉐이더
	//d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_ComputeRWResource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	int BlurCount = 1;
	for (int i = 0; i < BlurCount; ++i) {
		//m_CommandList->SetPipelineState(m_HorzComputePipelineState.Get());
		//m_CommandList->SetComputeRootSignature(m_ComputeRootSignature.Get());

		//m_RedShader.SetDescriptorHeaps(m_CommandList.Get());

		//m_pGBufferTexture->ComputeUpdateShaderVariables(m_CommandList.Get());
		//m_CommandList->SetComputeRootDescriptorTable(1, m_UAVGPUDescriptorHandle);

		//UINT groupX = (UINT)ceilf(FRAME_BUFFER_WIDTH / 256.F);
		//// UINT groupY = (UINT)ceilf(FRAME_BUFFER_HEIGHT / 32.F);
		//m_CommandList->Dispatch(groupX, FRAME_BUFFER_HEIGHT, 1);

		//m_CommandList->SetPipelineState(m_VertComputePipelineState.Get());
		//m_CommandList->SetComputeRootSignature(m_ComputeRootSignature.Get());

		//m_RedShader.SetDescriptorHeaps(m_CommandList.Get());

		//m_pGBufferTexture->ComputeUpdateShaderVariables(m_CommandList.Get());
		//m_CommandList->SetComputeRootDescriptorTable(1, m_UAVGPUDescriptorHandle);

		//UINT groupY = (UINT)ceilf(FRAME_BUFFER_HEIGHT / 256.F);
		//m_CommandList->Dispatch(FRAME_BUFFER_WIDTH, groupY, 1);
	}

	// d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_ComputeRWResource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON);

	////////////////////////////////////////////////////////// 계산쉐이더
}

void CGameFramework::RenderSwapChain()
{
	////////////////////////////////////////////////////// SwapChain
	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RenderTargetBuffers[m_SwapChainBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_CommandList->ClearRenderTargetView(m_SwapChainCPUHandle[m_SwapChainBufferIndex], /*pfClearColor*/Colors::Gray, 0, NULL);
	m_CommandList->ClearDepthStencilView(m_DepthStencilCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	m_CommandList->OMSetRenderTargets(1, &m_SwapChainCPUHandle[m_SwapChainBufferIndex], TRUE, &m_DepthStencilCPUHandle);

	// 장면을 렌더합니다.
	if (m_pScene) { 
		m_pScene->Render(m_CommandList.Get());
	}


#ifdef SHOW_G_BUFFERS
	for (int i = 0; i < m_GBuffersCount; ++i) {
		D3D12_VIEWPORT	GBuffer_Viewport{ 0 + G_BUFFER_WINDOW_WIDTH * i, FRAME_BUFFER_HEIGHT, G_BUFFER_WINDOW_WIDTH , G_BUFFER_WINDOW_HEIGHT, 0.0f, 1.0f };
		D3D12_RECT		ScissorRect{ 0 + G_BUFFER_WINDOW_WIDTH * i, FRAME_BUFFER_HEIGHT, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT + G_BUFFER_WINDOW_HEIGHT };


		m_RedShader.SetGraphicsRootSignature(m_CommandList.Get());
		m_CommandList->RSSetViewports(1, &GBuffer_Viewport);
		m_CommandList->RSSetScissorRects(1, &ScissorRect);

		// 픽셀 쉐이더만 바꾼다...
		if (i == 0) m_RedShader.OnPrepareRender(m_CommandList.Get());
		if (i == 1) m_GreenShader.OnPrepareRender(m_CommandList.Get());
		if (i == 2) m_BlueShader.OnPrepareRender(m_CommandList.Get());
		// 픽셀 쉐이더만 바꾼다...

		m_RedShader.SetDescriptorHeaps(m_CommandList.Get());
		m_pGBufferTexture->UpdateShaderVariables(m_CommandList.Get());
		// m_CommandList->SetGraphicsRootDescriptorTable(1, m_UAVGPUDescriptorHandle);

		m_CommandList->IASetVertexBuffers(0, 0, nullptr);
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->DrawInstanced(6, 1, 0, 0);
	}
#endif

#ifdef _TEXT_DRAW
	if (m_Testfont) m_Testfont->Render(m_CommandList.Get());
#endif

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RenderTargetBuffers[m_SwapChainBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//////////////////////////////////////////////////////// SwapChain
}
