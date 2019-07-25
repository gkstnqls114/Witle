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
	typedef void (CGameFramework::*renderFuncPtr)(); // 게임프레임워크의 렌더링 함수 포인터

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	 
	//MSAA 다중 샘플링을 활성화하고 다중 샘플링 레벨을 설정한다.
	bool m_bMsaa4xEnable = false;
	UINT m_nMsaa4xQualityLevels = 0;

	// Direct3D 디바이스 인터페이스에 대한 포인터이다.
	// 주로 리소스를 생성하기 위하여 필요하다.
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
	//// GBuffer 와 쉐도우맵을 위해 필요한 변수들 ////////////////////////////////////

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
	ID3D12Resource*				m_PlayerShadowmap; // 플레이어 위치에서 그려질 쉐도우맵
	D3D12_CPU_DESCRIPTOR_HANDLE m_ShadowmapCPUHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_PlayerShadowmapCPUHandle;

	UINT m_GBufferHeapCount{ m_GBuffersCount + m_ShadowmapCount + 1 /*uav*/};
	UINT m_GBufferForDepthIndex = m_GBufferHeapCount - 4;
	UINT m_GBufferForUAV = m_GBufferHeapCount - 3;
	UINT m_GBufferForShadowIndex = m_GBufferHeapCount - 2;
	UINT m_GBufferForPlayerShadowIndex = m_GBufferHeapCount - 1;
	MyDescriptorHeap* m_GBufferHeap{ nullptr }; // GBuffer 뿐만 아니라 Shadow도 담습니다.
	MyDescriptorHeap* m_ShadowmapHeap{ nullptr };  
	//// GBuffer 와 쉐도우맵을 위해 필요한 변수들 ////////////////////////////////////



	//// 컴퓨트 쉐이더를 위한 변수 ///////////////////////////////////////////
	  
	ID3D12Resource* m_ComputeRWResource; // 작성함 
	  
	// 블러를 위한 컴퓨트
	HorizonBlurShader* m_horizenShader{ nullptr };
	VerticalBlurShader* m_verticalShader{ nullptr };

	//// 컴퓨트 쉐이더를 위한 변수 ///////////////////////////////////////////

private:  
	//// OnCreate() 내부에서 사용되는 필수적으로 호출해야하는 함수들 ///////////
	 
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

	// 스왑 체인을 위한 깊이스텐실뷰와 렌더타겟뷰와는 다르게 사용될
	// GBuffer 리소스와 그를 위한 뷰를 생성합니다.
	void CreateGBufferView();
	
	// 쉐이더 맵 리소스와 그를 위한 뷰를 생성합니다.
	void CreateShadowmapView();

	void CreateCommandQueueAndList();

	//// OnCreate() 내부에서 사용되는 필수적으로 호출해야하는 함수들 ///////////



	//// CPU와 GPU를 동기화하는 함수 /////////////////////////////////////////
	
	void WaitForGpuComplete();
	void MoveToNextFrame();
	
	//// CPU와 GPU를 동기화하는 함수 /////////////////////////////////////////


	//렌더링할 메쉬와 게임 객체를 생성하고 소멸하는 함수이다.
	void BuildObjects();
	void ReleaseObjects();

	//윈도우 클라이언트 영역의 크기가 바뀌면 실행하는 함수.
	void OnResizeBackBuffers();

	// Render() 내부에서 사용하는 함수들이다.	

	// 현재 GBuffer 를 화면 하단에 렌더합니다.
	void RenderGBuffers();

	// 계산 쉐이더를 통해 블러링합니다.
	void Blur();
	
	// SwapChain에 현재 장면을 렌더링합니다.
	void RenderSwapChain();
	
	// SwapChain에 디퍼드 쉐이더를 통해 현재 장면을 렌더링합니다.
	void DefferedRenderSwapChain();

	void RenderToTexture();

	// Shadow 맵만 렌더링합니다.
	void RenderForShadow();

	// 플레이어 Shadow 맵만 렌더링합니다.
	void RenderForPlayerShadow();

	// Compute 쉐이더를 통해 hdr을 위한 텍스쳐를 계산합니다.
	void ComputeHDR();

	// 키보드와 마우스 메시지를 처리하는 부분이다.
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	// 깊이 버퍼와 그릴 렌더타겟을 하나 골라 장면을 렌더링합니다.
	void RenderOnRT(renderFuncPtr renderPtr,ID3D12Resource* pRendertargetResource, D3D12_CPU_DESCRIPTOR_HANDLE renderTarget, D3D12_CPU_DESCRIPTOR_HANDLE depthStencil);
	
	// 깊이 버퍼와 그릴 렌더타겟을 여러 개 골라 장면을 렌더링합니다.
	void RenderOnRTs(renderFuncPtr renderPtr, UINT RenderTargetCount,ID3D12Resource** resource, D3D12_CPU_DESCRIPTOR_HANDLE* renderTarget, D3D12_CPU_DESCRIPTOR_HANDLE depthStencil);

	// GBuffer에 장면을 렌더링합니다.
	void RenderOnGbuffer();
	 
	// 렌더했던 Gbuffer를 이용하여 렌더링합니다.
	void DefferedRenderOnSwapchain();

	// 쉐도우 맵을 그립니다.
	void RenderShadowMap();

	// 필요한 쉐이더를 빌드합니다.
	void BuildShaders(); 
	 
public:
	CGameFramework();
	~CGameFramework();

	// 프레임워크를 초기화하는 함수입니다 (주 윈도우가 생성되면 호출)
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);

	// 게임 종료시 호출되는 함수입니다.
	void OnDestroy();

	//프레임워크의 핵심(사용자 입력, 애니메이션, 업데이트)을 구성하는 함수입니다. 
	void UpdateGamelogic(float );

	// 렌더링을 처리하는 함수입니다.
	void Render();

	// 윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수입니다.
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
private: 
	// 현재 사용하는 장면을 관리하는 매니저입니다
	SceneMgr *m_SceneMgr{ nullptr }; 
};

