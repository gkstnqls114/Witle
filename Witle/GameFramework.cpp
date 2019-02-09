#include "stdafx.h"
#include "ShaderManager.h"
#include "GameTimer.h"
#include "GameInput.h"
#include "d3dUtil.h"
#include "CubeShader.h"
#include "TESTShader.h"
#include "TerrainShader.h"
#include "GameScreen.h" 
#include "GameScene.h"

#include "GameFramework.h"


void CGameFramework::Render()
{
	HRESULT hResult = m_pd3dCommandAllocator->Reset();
	hResult = m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	RenderSwapChain();

	hResult = m_pd3dCommandList->Close();

	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

#ifdef _WITH_PRESENT_PARAMETERS
	DXGI_PRESENT_PARAMETERS dxgiPresentParameters;
	dxgiPresentParameters.DirtyRectsCount = 0;
	dxgiPresentParameters.pDirtyRects = NULL;
	dxgiPresentParameters.pScrollRect = NULL;
	dxgiPresentParameters.pScrollOffset = NULL;
	m_pdxgiSwapChain->Present1(1, 0, &dxgiPresentParameters);
#else
#ifdef _WITH_SYNCH_SWAPCHAIN
	m_pdxgiSwapChain->Present(1, 0);
#else
	m_pdxgiSwapChain->Present(0, 0);
#endif
#endif

	//	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
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

#if defined(_DEBUG)
	if (m_pd3dDebugController) m_pd3dDebugController->Release();
#endif

	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDsvDescriptorHeap) m_pd3dDsvDescriptorHeap->Release();

	for (int i = 0; i < m_nSwapChainBuffers; i++) if (m_ppd3dSwapChainBackBuffers[i]) m_ppd3dSwapChainBackBuffers[i]->Release();
	if (m_pd3dRtvDescriptorHeap) m_pd3dRtvDescriptorHeap->Release();

	if (m_pd3dCommandAllocator) m_pd3dCommandAllocator->Release();
	if (m_pd3dCommandQueue) m_pd3dCommandQueue->Release();
	if (m_pd3dCommandList) m_pd3dCommandList->Release();

	if (m_pd3dFence) m_pd3dFence->Release();

	m_pdxgiSwapChain->SetFullscreenState(FALSE, NULL);
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiFactory) m_pdxgiFactory->Release();
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

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

#ifdef _WITH_CREATE_SWAPCHAIN_FOR_HWND
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	dxgiSwapChainDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.Scaling = DXGI_SCALING_NONE;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed = TRUE;

	HRESULT hResult = m_pdxgiFactory->CreateSwapChainForHwnd(m_pd3dCommandQueue, m_hWnd, &dxgiSwapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1 **)&m_pdxgiSwapChain);
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE;
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	HRESULT hResult = m_pdxgiFactory->CreateSwapChain(m_pd3dCommandQueue, &dxgiSwapChainDesc, (IDXGISwapChain **)&m_pdxgiSwapChain);
#endif

	if (!m_pdxgiSwapChain)
	{
		MessageBox(NULL, L"Swap Chain Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	hResult = m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
}

void CGameFramework::CreateDirect3DDevice()
{

	HRESULT hResult;

	UINT nDXGIFactoryFlags = 0;
#if defined(_DEBUG)
	ID3D12Debug *pd3dDebugController = NULL;
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void **)&pd3dDebugController);
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	hResult = ::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void **)&m_pdxgiFactory);

	IDXGIAdapter1 *pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void **)&m_pd3dDevice))) break;
	}

	if (!m_pd3dDevice)
	{
		hResult = m_pdxgiFactory->EnumWarpAdapter(_uuidof(IDXGIAdapter1), (void **)&pd3dAdapter);
		hResult = D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void **)&m_pd3dDevice);
	}

	if (!m_pd3dDevice)
	{
		MessageBox(NULL, L"Direct3D 12 Device Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	hResult = m_pd3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;

	hResult = m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void **)&m_pd3dFence);
	for (UINT i = 0; i < m_nSwapChainBuffers; i++) m_nFenceValues[i] = 0;

	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	d3dUtil::gnCbvSrvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	if (pd3dAdapter) pd3dAdapter->Release();
}

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = m_nSwapChainBuffers;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HRESULT hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dRtvDescriptorHeap);
	m_nRtvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dDsvDescriptorHeap);
	m_nDsvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void CGameFramework::CreateCommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hResult = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void **)&m_pd3dCommandQueue);

	hResult = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&m_pd3dCommandAllocator);

	hResult = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&m_pd3dCommandList);
	hResult = m_pd3dCommandList->Close();
}

void CGameFramework::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < m_nSwapChainBuffers; i++)
	{
		m_pdxgiSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void **)&m_ppd3dSwapChainBackBuffers[i]);
		m_pd3dDevice->CreateRenderTargetView(m_ppd3dSwapChainBackBuffers[i], NULL, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += m_nRtvDescriptorIncrementSize;
	}
}

void CGameFramework::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = m_nWndClientWidth;
	d3dResourceDesc.Height = m_nWndClientHeight;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapProperties.CreationNodeMask = 1;
	d3dHeapProperties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dDevice->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void **)&m_pd3dDepthStencilBuffer);

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	//	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, NULL, d3dDsvCPUDescriptorHandle);
	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);
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
//		m_d3dDevice->CreateRenderTargetViews(m_pGBufferTexture->GetTexture(i), &d3dRenderTargetViewDesc, m_GBufferCPUHandle[i]);
//		d3dRtvCPUDescriptorHandle.ptr += m_RtvDescriptorSize;
//	}
//
//
//}

void CGameFramework::BuildObjects()
{
	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);
	
	///////////////////////////////////////////////////////////////////////////// 리소스 생성
	m_pScene = new GameScene;
	if (m_pScene) m_pScene->BuildObjects(m_pd3dDevice, m_pd3dCommandList);
	
	Shader* pCubeShader = new CubeShader();
	pCubeShader->CreateShader(m_pd3dDevice, m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("Cube", pCubeShader);

	Shader* pTerrainShader = new TerrainShader();
	pTerrainShader->CreateShader(m_pd3dDevice, m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("Terrain", pTerrainShader); 

	m_TESTShader = new TESTShader();
	m_TESTShader->CreateShader(m_pd3dDevice, m_pScene->GetGraphicsRootSignature());
	///////////////////////////////////////////////////////////////////////////// 리소스 생성

	m_pd3dCommandList->Close();
	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

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
	const UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CGameFramework::MoveToNextFrame()
{
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	//m_nSwapChainBufferIndex = (m_nSwapChainBufferIndex + 1) % m_nSwapChainBuffers;

	UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
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
			m_pdxgiSwapChain->GetFullscreenState(&bFullScreenState, NULL);
			m_pdxgiSwapChain->SetFullscreenState(!bFullScreenState, NULL);

			DXGI_MODE_DESC dxgiTargetParameters;
			dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dxgiTargetParameters.Width = m_nWndClientWidth;
			dxgiTargetParameters.Height = m_nWndClientHeight;
			dxgiTargetParameters.RefreshRate.Numerator = 60;
			dxgiTargetParameters.RefreshRate.Denominator = 1;
			dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			m_pdxgiSwapChain->ResizeTarget(&dxgiTargetParameters);

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
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::OnResizeBackBuffers()
{
	WaitForGpuComplete();

	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	for (int i = 0; i < m_nSwapChainBuffers; i++) if (m_ppd3dSwapChainBackBuffers[i]) m_ppd3dSwapChainBackBuffers[i]->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();

#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
#else
	//m_pdxgiSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(m_nSwapChainBuffers, m_nWndClientWidth, m_nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
#endif
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

	CreateRenderTargetViews();
	CreateDepthStencilView();

	m_pd3dCommandList->Close();

	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

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
	//d3dUtil::SynchronizeResourceTransition(m_pd3dCommandList, m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex];
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_nSwapChainBufferIndex * m_nRtvDescriptorIncrementSize);

	float pfClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pd3dCommandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, pfClearColor/*Colors::Azure*/, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	m_pd3dCommandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);

	//// 장면을 렌더합니다.
	//if (m_pScene) { 
	//	m_pScene->Render(m_pd3dCommandList);
	//}
	D3D12_VIEWPORT m_d3dViewport;
	D3D12_RECT m_d3dScissorRect;

	m_d3dViewport.TopLeftX = 0;
	m_d3dViewport.TopLeftY = 0;
	m_d3dViewport.Width = static_cast<float>(m_nWndClientWidth);
	m_d3dViewport.Height = static_cast<float>(m_nWndClientHeight);
	m_d3dViewport.MinDepth = 0.0f;
	m_d3dViewport.MaxDepth = 1.0f;
	m_d3dScissorRect = { 0, 0, m_nWndClientWidth, m_nWndClientHeight };

	m_pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	m_pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);


	//그래픽 루트 시그너쳐를 설정한다.
	m_pd3dCommandList->SetGraphicsRootSignature(m_pScene->GetGraphicsRootSignature());
	//파이프라인 상태를 설정한다.
	m_pd3dCommandList->SetPipelineState(m_TESTShader->GetPSO());
	//프리미티브 토폴로지(삼각형 리스트)를 설정한다.
	m_pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//정점 3개를 사용하여 렌더링한다.
	m_pd3dCommandList->DrawInstanced(3, 1, 0, 0);


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

	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);


	//d3dUtil::SynchronizeResourceTransition(m_pd3dCommandList, m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//////////////////////////////////////////////////////// SwapChain
}
