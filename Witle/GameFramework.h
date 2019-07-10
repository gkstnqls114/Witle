#pragma once
#include "MeshRenderer.h"

class Scene;
class Texture;
class ComputeShader;
class MyDescriptorHeap;

class CLoadedModelInfo;

class CGameFramework
{

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	
	//int m_nWndClientWidth{ 0 };	 
	//int m_nWndClientHeight{ 0 };


	//MSAA ���� ���ø��� Ȱ��ȭ�ϰ� ���� ���ø� ������ �����Ѵ�.
	bool m_bMsaa4xEnable = false;
	UINT m_nMsaa4xQualityLevels = 0;

	// Direct3D ����̽� �������̽��� ���� �������̴�.
	// �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	ComPtr<ID3D12Device> m_d3dDevice;

	ComPtr<IDXGIFactory4> m_dxgiFactory;

	ComPtr<IDXGISwapChain3> m_SwapChain;
	static const UINT m_SwapChainBuffersCount{ 2 };
	D3D12_CPU_DESCRIPTOR_HANDLE m_SwapChainCPUHandle[m_SwapChainBuffersCount];
	UINT m_SwapChainBufferIndex{ 0 };

	ID3D12Resource *m_RenderTargetBuffers[m_SwapChainBuffersCount];
	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	UINT m_RtvDescriptorSize;
	
	ID3D12Resource *m_DepthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_DepthStencilCPUHandle;
	UINT m_DsvDescriptorSize;
	 
	ComPtr<ID3D12CommandQueue> m_CommandQueue;
	ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	
	ComPtr<ID3D12Fence> m_Fence;
	UINT64 m_nFenceValues[m_SwapChainBuffersCount];
	HANDLE m_hFenceEvent{ NULL };

#if defined(_DEBUG)
	ComPtr<ID3D12Debug> m_pd3dDebugController;
	ComPtr<ID3D12DebugDevice> m_pd3dDebugDevice;
#endif
	
private:  
	//// GBuffer �� ������� ////////////////////////////////////
	static const UINT m_GBuffersCountForDepth{ 1 };
	static const UINT m_ShadowmapCount{ 1 };
	static const UINT m_GBuffersCountForRenderTarget{ 3 };
	const UINT m_DsvDescriptorsCount{ 1 + m_GBuffersCountForDepth + m_ShadowmapCount };
	static const UINT m_GBuffersCount{ m_GBuffersCountForDepth + m_GBuffersCountForRenderTarget };

	ID3D12Resource*				m_GBuffersForRenderTarget[m_GBuffersCountForRenderTarget];
	D3D12_CPU_DESCRIPTOR_HANDLE m_GBufferCPUHandleForRenderTarget[m_GBuffersCountForRenderTarget];

	ID3D12Resource*				m_GBuffersForDepth[m_GBuffersCountForDepth];
	D3D12_CPU_DESCRIPTOR_HANDLE m_GBufferCPUHandleForDepth[m_GBuffersCountForDepth];

	ID3D12Resource*				m_Shadowmap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_ShadowmapCPUHandle;
	//// GBuffer �� ������� ////////////////////////////////////

	UINT m_GBufferDescriptorSize;
	
	float	m_GBufferClearValue[3][4]{
		{ 0.f, 0.f, 0.f, 1.f },
		{ 0.f, 1.f, 0.f, 1.f },
		{ 0.f, 0.f, 1.f, 1.f }
	};

	///////////////////// ��ǻƮ ���̴��� ���� ����
	float	m_RWClearValue[4] = { 1.f, 0.f, 1.f, 0.f };
	ID3D12Resource* m_ComputeRWResource; // �ۼ��� 

	const UINT m_UAVParameterIndex = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE m_UAVGPUDescriptorHandle;

private:  
	//���� ü��, ����̽�, ������ ��, ��� ť/�Ҵ���/����Ʈ�� �����ϴ� �Լ��̴�.
	// OnCreate() ���ο��� ����Ѵ�.
	void CreateRWResourceViews() ;
	
	void ReleaseSwapChainBuffer();
	void ReleaseGBuffers();
	void ReleaseDepthStencilBuffer();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateGBufferView(); // MRT�� ���� ����
	void CreateCommandQueueAndList();

	//CPU�� GPU�� ����ȭ�ϴ� �Լ��̴�.
	void WaitForGpuComplete();
	void MoveToNextFrame();

	//�������� �޽��� ���� ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�.
	void BuildObjects();
	void ReleaseObjects();

	//������ Ŭ���̾�Ʈ ������ ũ�Ⱑ �ٲ�� �����ϴ� �Լ�.
	void OnResizeBackBuffers();

	// Render() ���ο��� ����ϴ� �Լ����̴�.	
	void RenderGBuffers(); // GBuffer�� Render
	void Blur(); // ��꽦�̴��� ���� �����Ѵ�.
	void RenderSwapChain(); // SwapChain�� Render
	void DefferedRenderSwapChain(); // SwapChain�� Render

	// Ű����� ���콺 �޽����� ó���ϴ� �κ��̴�.
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	void RenderOnGbuffer();
	void RenderOnCompute();
	void RenderOnSwapchain();
	void DefferedRenderOnSwapchain();

	void BuildShaders();
	void BuildTESTObjects();
	  
public:
	CGameFramework();
	~CGameFramework();

	//�����ӿ�ũ�� �ʱ�ȭ�ϴ� �Լ��̴�(�� �����찡 �����Ǹ� ȣ��ȴ�).
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	//�����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������Ʈ)�� �����ϴ� �Լ��̴�. 
	void UpdateGamelogic(float );

	// �������� ó���ϴ� �Լ��̴�.
	void Render();

	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�.
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
private: 
	
	Scene *m_pScene{ nullptr };
	 
	// ���� ���� ��ǻƮ
	ComputeShader* m_horizenShader{ nullptr };
	ComputeShader* m_verticalShader{ nullptr };

	MyDescriptorHeap* m_GBufferHeap{ nullptr };
	MyDescriptorHeap* m_DefferedRenderHeap{ nullptr };
};

