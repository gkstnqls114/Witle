#include "stdafx.h"
#include "d3dUtil.h"

#include "ShaderManager.h"
#include "GameTimer.h"
#include "GameInput.h"

#include "TESTShader.h"
#include "CubeShader.h"

#ifdef _SHOW_BOUNDINGBOX
#include "LineShader.h"
#endif // _SHOW_BOUNDINGBOX
 
#include "Object.h"
#include "StandardShader.h"
#include "TerrainShader.h"
#include "SkinnedShader.h"
#include "HorizonBlurShader.h"
#include "VerticalBlurShader.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"

#include "GameScreen.h" 
#include "GameScene.h"

#include "GameFramework.h"


void CGameFramework::Render()
{
	HRESULT hResult = m_CommandAllocator->Reset();
	hResult = m_CommandList->Reset(m_CommandAllocator.Get(), NULL);

	//// GBuffer�� Render //////////////////////////
#ifdef CHECK_SUBVIEWS
	RenderOnGbuffer();
#endif
	//// GBuffer�� Render //////////////////////////

	//// ComputeShader ////////////////////////// 
	//// ComputeShader ////////////////////////// 
	
	//// SwapChain�� Render //////////////////////////
	RenderOnSwapchain();
	//// SwapChain�� Render //////////////////////////


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
	
	CreateRWResourceViews();
	 
	BuildObjects();
	 
	GameScreen::SetSwapChain(m_SwapChain.Get());

	return true;
}

void CGameFramework::OnDestroy()
{
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

	if (m_pShadowMap)
	{
		m_pShadowMap->Release();
	}

	if (m_horizenShader)
	{
		delete m_horizenShader;
	}
	if (m_verticalShader)
	{
		delete m_verticalShader;
	}
}

void CGameFramework::RenderShadowMap()
{
	m_CommandList->OMSetRenderTargets(1, &m_SwapChainCPUHandle[m_SwapChainBufferIndex], TRUE, &m_DepthStencilCPUHandle);

	////�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	m_CommandList->SetGraphicsRootSignature(m_pScene->GetGraphicsRootSignature());

	////���������� ���¸� �����Ѵ�.
	m_CommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("TEST")->GetPSO());
	
	float anotherWidth = static_cast<float>(GameScreen::GetAnotherWidth());
	float anotherHeight = static_cast<float>(GameScreen::GetAnotherHeight());
	float width = static_cast<float>(GameScreen::GetAnotherWidth());
	float height = static_cast<float>(GameScreen::GetHeight());

	// static_cast<GameScene*>(m_pScene)->TESTSetRootDescriptor(m_CommandList.Get());
	m_TESTHeap_1->FirstUpdate(m_CommandList.Get());

	// ���ҽ��� �ٲ۴�.. 
	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_TESTHeap_1->GetGPUSrvDescriptorStartHandle();
	
	// ����� �����մϴ�.
	for (int i = 0; i < m_GBuffersCount; ++i)
	{
		D3D12_VIEWPORT	GBuffer_Viewport{ anotherWidth * ( 2 +i), height, anotherWidth , anotherHeight, 0.0f, 1.0f };
		D3D12_RECT		ScissorRect{ anotherWidth * (2+i), height, anotherWidth * (3 + i) , height + anotherHeight };

		m_CommandList->RSSetViewports(1, &GBuffer_Viewport);
		m_CommandList->RSSetScissorRects(1, &ScissorRect);

		m_CommandList->SetGraphicsRootDescriptorTable(5, handle);
		handle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->DrawInstanced(6, 1, 0, 0);
	}
}


void CGameFramework::CreateRWResourceViews()
{
	D3D12_CLEAR_VALUE d3dClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM,
	{m_RWClearValue[0], m_RWClearValue[1], m_RWClearValue[2], m_RWClearValue[3] } };

	// ���ҽ� ����
	// ���۵� �ƴϰ�, ���ҽ� ���°� ���� ���ٽǵ� ���� Ÿ�ٵ� �ƴ϶�� Ŭ�������� NULL
	m_ComputeRWResource = d3dUtil::CreateTexture2DResource
		(m_d3dDevice.Get(), m_CommandList.Get(), 
			GameScreen::GetWidth(), GameScreen::GetHeight(),
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS/*�ٸ�*/,
			D3D12_RESOURCE_STATE_COMMON/*�ٸ�*/,
			NULL/*, 0*/ /*�ε���*/); 
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
	OutputDebugString(L"...EnableDebugLayer\n");
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


	if (pAdapter) pAdapter->Release();
}

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	HRESULT hResult;

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = m_SwapChainBuffersCount + m_GBuffersCount;
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
	m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue, IID_PPV_ARGS(&m_DepthStencilBuffer));
	m_DepthStencilCPUHandle = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer, &d3dDepthStencilViewDesc, m_DepthStencilCPUHandle);

	//���� ���� ������ ������ ���� �����Ѵ�.
	m_d3dDevice->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &ClearValue, IID_PPV_ARGS(&m_pShadowMap));
	m_ShadowMapCPUHandle = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_ShadowMapCPUHandle.ptr += m_DsvDescriptorSize;
	m_d3dDevice->CreateDepthStencilView(m_pShadowMap, &d3dDepthStencilViewDesc, m_ShadowMapCPUHandle);
}

void CGameFramework::CreateGBufferView()
{ 
	for (UINT i = 0; i < m_GBuffersCount; i++)
	{
		D3D12_CLEAR_VALUE d3dClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM,
		{m_GBufferClearValue[i][0], m_GBufferClearValue[i][1], m_GBufferClearValue[i][2], m_GBufferClearValue[i][3] } };
		
		// �ش� �ؽ��ĸ� 2�� �ؽ��� ��̷� ����?? ���ۿ� ��´�.
		m_GBuffers[i] =
			d3dUtil::CreateTexture2DResource(
				m_d3dDevice.Get(), m_CommandList.Get(),
				GameScreen::GetClientWidth(), GameScreen::GetClientHeight(),
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
				D3D12_RESOURCE_STATE_GENERIC_READ, &d3dClearValue
			);
	}

	// ��ũ���� �ڵ� ����
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_SwapChainBuffersCount * m_RtvDescriptorSize);

	// ����Ÿ�� �� ����
	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
	d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;

	for (UINT i = 0; i < m_GBuffersCount; i++)
	{
		// ����Ÿ�� �� ����
		m_GBufferCPUHandle[i] = d3dRtvCPUDescriptorHandle;
		m_GBufferCPUHandle[i].ptr += (m_RtvDescriptorSize * i);

		// ����̽��� ����Ÿ�ٺ並 �ش� �ؽ��ķ� �����ȴ�...
		m_d3dDevice->CreateRenderTargetView(m_GBuffers[i], &d3dRenderTargetViewDesc, m_GBufferCPUHandle[i]);
	}

	if (!m_TESTHeap_1)
	{
		m_TESTHeap_1 = new MyDescriptorHeap;
		m_TESTHeap_1->CreateCbvSrvUavDescriptorHeaps(m_d3dDevice.Get(), m_CommandList.Get(), 0, m_GBuffersCount, 0);
		for (int i = 0; i < m_GBuffersCount; ++i)
		{
			m_TESTHeap_1->CreateShaderResourceViews(m_d3dDevice.Get(), m_CommandList.Get(), m_GBuffersCount, m_GBuffers[i], RESOURCE_TEXTURE2D, i);
		}
	}
}

void CGameFramework::BuildObjects()
{
	HRESULT hResult;

	m_CommandList->Reset(m_CommandAllocator.Get(), NULL);
	
	///////////////////////////////////////////////////////////////////////////// ���ҽ� ����
	m_pScene = new GameScene;
	if (m_pScene) m_pScene->BuildObjects(m_d3dDevice.Get(), m_CommandList.Get());
	
	BuildTESTObjects();
	BuildShaders();

	///////////////////////////////////////////////////////////////////////////// ���ҽ� ����

	hResult = m_CommandList->Close();
	assert(hResult == S_OK);
	ID3D12CommandList *ppd3dCommandLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	
	WaitForGpuComplete();

	///////////////////////////////////////////////////////////////////////////// ���ε� �� ������
	if (m_pScene) m_pScene->ReleaseUploadBuffers(); 
	///////////////////////////////////////////////////////////////////////////// ���ε� �� ������

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
		m_pScene->AnimateObjects(fElapsedTime);
		m_pScene->LastUpdate(fElapsedTime);

		// ���� �ִϸ��̼� ���ٰ� ������ �𸣰ڴ�.
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
			if (GameScreen::GetHeight() == 540)
			{
				GameScreen::ChangeWindowScreen(800, 600);
			}
			else
			{
				GameScreen::ChangeWindowScreen(960, 540);
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
	m_CommandList->ClearDepthStencilView(m_ShadowMapCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	// �ϴܿ� �׽�Ʈ������ ���� ���ҽ����� �����Ѵ�.
	RenderShadowMap();
	
	// �⺻ ���� ����� �����Ѵ�.
	RenderSwapChain();

	d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_RenderTargetBuffers[m_SwapChainBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void CGameFramework::BuildShaders()
{
	CMaterial::PrepareShaders(m_d3dDevice.Get(), m_CommandList.Get(), m_pScene->GetGraphicsRootSignature());

	Shader* pCubeShader = new CubeShader();
	pCubeShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("Cube", pCubeShader);

	Shader* pTerrainShader = new TerrainShader();
	pTerrainShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("Terrain", pTerrainShader);
	 
	Shader* pStandardShader = new StandardShader();
	pStandardShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("StandardShader", pStandardShader);

	Shader* pSkinnedShader = new SkinnedShader();
	pSkinnedShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("SkinnedShader", pSkinnedShader);
#ifdef _SHOW_BOUNDINGBOX
	Shader* pLineShader = new LineShader();
	pLineShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("Line", pLineShader);
#endif
}

void CGameFramework::BuildTESTObjects()
{
	m_horizenShader = new HorizonBlurShader();
	m_horizenShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());

	m_verticalShader = new VerticalBlurShader();
	m_verticalShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());

	Shader* pTESTShader = new TESTShader();
	pTESTShader->CreateShader(m_d3dDevice.Get(), m_pScene->GetGraphicsRootSignature());
	ShaderManager::GetInstance()->InsertShader("TEST", pTESTShader);
}

void CGameFramework::RenderOnGbuffer()
{
	for (int i = 0; i < m_GBuffersCount; i++) { 
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_GBuffers[i], D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ClearRenderTargetView(m_GBufferCPUHandle[i], m_GBufferClearValue[i], 0, NULL);
	}
 
	m_CommandList->ClearDepthStencilView(m_DepthStencilCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	m_CommandList->OMSetRenderTargets(m_GBuffersCount, m_GBufferCPUHandle, TRUE, &m_DepthStencilCPUHandle);

	if (m_pScene) {
		m_pScene->Render(m_CommandList.Get());
	}

	for (int x = 0; x < m_GBuffersCount; ++x)
	{
		d3dUtil::SynchronizeResourceTransition(m_CommandList.Get(), m_GBuffers[x], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
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
	
	for (int i = 0; i < m_SwapChainBuffersCount; i++) {
		if (m_RenderTargetBuffers[i])
		{
			m_RenderTargetBuffers[i]->Release();
		}
	}

	if (m_DepthStencilBuffer) {
		m_DepthStencilBuffer->Release();
	}

	if (m_pShadowMap) {
		m_pShadowMap->Release();
	}

	for (int i = 0; i < m_GBuffersCount; ++i)
	{
		if (m_GBuffers[i])
		{
			m_GBuffers[i]->Release();
		}
	}
	if (m_TESTHeap_1)
	{
		delete m_TESTHeap_1;
		m_TESTHeap_1 = nullptr;
	}

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

	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateGBufferView();
	
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
	m_CommandList->OMSetRenderTargets(1, &m_SwapChainCPUHandle[m_SwapChainBufferIndex], TRUE, &m_DepthStencilCPUHandle);

	// ����� �����մϴ�.
	if (m_pScene) { 
		m_pScene->Render(m_CommandList.Get());
	}

}
