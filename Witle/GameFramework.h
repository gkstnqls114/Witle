#pragma once
#include "d3dx12.h"
#include "MeshRenderer.h"
 
class SceneMgr; 

class Texture;
class ComputeShader;
class HorizonBlurShader;
class VerticalBlurShader;
class MyDescriptorHeap;
class CLoadedModelInfo;

class CGameFramework
{ 
	typedef void (CGameFramework::*renderFuncPtr)(); // ���������ӿ�ũ�� ������ �Լ� ������

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	 
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
	//// GBuffer �� ��������� ���� �ʿ��� ������ ////////////////////////////////////

	float	m_clearValueColor[4]
	{
		0.5f, 0.f, 0.f, 1.f
	};

	static const UINT m_GBuffersCountForDepth{ 1 };
	static const UINT m_ShadowmapCount{ 2 };
	static const UINT m_GBuffersCountForRenderTarget{ 3 };
	const UINT m_DsvDescriptorsCount{ 1 + m_GBuffersCountForDepth + m_ShadowmapCount };
	static const UINT m_GBuffersCount{ m_GBuffersCountForDepth + m_GBuffersCountForRenderTarget };

	ID3D12Resource*				m_GBuffersForRenderTarget[m_GBuffersCountForRenderTarget];
	D3D12_CPU_DESCRIPTOR_HANDLE m_GBufferCPUHandleForRenderTarget[m_GBuffersCountForRenderTarget];

	ID3D12Resource*				m_GBuffersForDepth[m_GBuffersCountForDepth];
	D3D12_CPU_DESCRIPTOR_HANDLE m_GBufferCPUHandleForDepth[m_GBuffersCountForDepth];

	ID3D12Resource*				m_Shadowmap;
	ID3D12Resource*				m_PlayerShadowmap; // �÷��̾� ��ġ���� �׷��� �������
	D3D12_CPU_DESCRIPTOR_HANDLE m_ShadowmapCPUHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_PlayerShadowmapCPUHandle;

	UINT m_GBufferHeapCount{ m_GBuffersCount + m_ShadowmapCount + 1 /*uav*/};
	UINT m_GBufferForDepthIndex = m_GBufferHeapCount - 4;
	UINT m_GBufferForUAV = m_GBufferHeapCount - 3;
	UINT m_GBufferForShadowIndex = m_GBufferHeapCount - 2;
	UINT m_GBufferForPlayerShadowIndex = m_GBufferHeapCount - 1;
	MyDescriptorHeap* m_GBufferHeap{ nullptr }; // GBuffer �Ӹ� �ƴ϶� Shadow�� ����ϴ�.
	MyDescriptorHeap* m_ShadowmapHeap{ nullptr };  
	//// GBuffer �� ��������� ���� �ʿ��� ������ ////////////////////////////////////



	//// ��ǻƮ ���̴��� ���� ���� ///////////////////////////////////////////
	  
	ID3D12Resource* m_ComputeRWResource; // �ۼ��� 
	  
	// ���� ���� ��ǻƮ
	HorizonBlurShader* m_horizenShader{ nullptr };
	VerticalBlurShader* m_verticalShader{ nullptr };

	//// ��ǻƮ ���̴��� ���� ���� ///////////////////////////////////////////

private:  
	//// OnCreate() ���ο��� ���Ǵ� �ʼ������� ȣ���ؾ��ϴ� �Լ��� ///////////
	 
	void ReleaseSwapChainBuffer();
	void ReleaseGBuffers();
	void ReleaseDepthStencilBuffer();
	void ReleaseShadowmap();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetView();
	void CreateDepthStencilView(); 
	void CreateRWResourceViews(); 

	// ���� ü���� ���� ���̽��ٽǺ�� ����Ÿ�ٺ�ʹ� �ٸ��� ����
	// GBuffer ���ҽ��� �׸� ���� �並 �����մϴ�.
	void CreateGBufferView();
	
	// ���̴� �� ���ҽ��� �׸� ���� �並 �����մϴ�.
	void CreateShadowmapView();

	void CreateCommandQueueAndList();

	//// OnCreate() ���ο��� ���Ǵ� �ʼ������� ȣ���ؾ��ϴ� �Լ��� ///////////



	//// CPU�� GPU�� ����ȭ�ϴ� �Լ� /////////////////////////////////////////
	
	void WaitForGpuComplete();
	void MoveToNextFrame();
	
	//// CPU�� GPU�� ����ȭ�ϴ� �Լ� /////////////////////////////////////////


	//�������� �޽��� ���� ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�.
	void BuildObjects();
	void ReleaseObjects();

	//������ Ŭ���̾�Ʈ ������ ũ�Ⱑ �ٲ�� �����ϴ� �Լ�.
	void OnResizeBackBuffers();

	// Render() ���ο��� ����ϴ� �Լ����̴�.	

	// ���� GBuffer �� ȭ�� �ϴܿ� �����մϴ�.
	void RenderGBuffers();

	// ��� ���̴��� ���� �����մϴ�.
	void Blur();
	
	// SwapChain�� ���� ����� �������մϴ�.
	void RenderSwapChain();
	
	// SwapChain�� ���۵� ���̴��� ���� ���� ����� �������մϴ�.
	void DefferedRenderSwapChain();

	void RenderToTexture();

	// Shadow �ʸ� �������մϴ�.
	void RenderForShadow();

	// �÷��̾� Shadow �ʸ� �������մϴ�.
	void RenderForPlayerShadow();

	// Compute ���̴��� ���� hdr�� ���� �ؽ��ĸ� ����մϴ�.
	void ComputeHDR();

	// Ű����� ���콺 �޽����� ó���ϴ� �κ��̴�.
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	// ���� ���ۿ� �׸� ����Ÿ���� �ϳ� ��� ����� �������մϴ�.
	void RenderOnRT(renderFuncPtr renderPtr,ID3D12Resource* pRendertargetResource, D3D12_CPU_DESCRIPTOR_HANDLE renderTarget, D3D12_CPU_DESCRIPTOR_HANDLE depthStencil);
	
	// ���� ���ۿ� �׸� ����Ÿ���� ���� �� ��� ����� �������մϴ�.
	void RenderOnRTs(renderFuncPtr renderPtr, UINT RenderTargetCount,ID3D12Resource** resource, D3D12_CPU_DESCRIPTOR_HANDLE* renderTarget, D3D12_CPU_DESCRIPTOR_HANDLE depthStencil);

	// GBuffer�� ����� �������մϴ�.
	void RenderOnGbuffer();
	 
	// �����ߴ� Gbuffer�� �̿��Ͽ� �������մϴ�.
	void DefferedRenderOnSwapchain();

	// ������ ���� �׸��ϴ�.
	void RenderShadowMap();

	// �ʿ��� ���̴��� �����մϴ�.
	void BuildShaders(); 
	 
public:
	CGameFramework();
	~CGameFramework();

	// �����ӿ�ũ�� �ʱ�ȭ�ϴ� �Լ��Դϴ� (�� �����찡 �����Ǹ� ȣ��)
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);

	// ���� ����� ȣ��Ǵ� �Լ��Դϴ�.
	void OnDestroy();

	//�����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������Ʈ)�� �����ϴ� �Լ��Դϴ�. 
	void UpdateGamelogic(float );

	// �������� ó���ϴ� �Լ��Դϴ�.
	void Render();

	// �������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��Դϴ�.
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
private: 
	// ���� ����ϴ� ����� �����ϴ� �Ŵ����Դϴ�
	SceneMgr *m_SceneMgr{ nullptr }; 
};

