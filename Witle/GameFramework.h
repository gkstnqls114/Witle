#pragma once
#include "MeshRenderer.h"
#define _WITH_PLAYER_TOP

class Scene;
class Texture;
class ComputeShader;
class MyDescriptorHeap;

class CGameFramework
{

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	
	//int m_nWndClientWidth{ 0 };	 
	//int m_nWndClientHeight{ 0 };


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
	////////////////////////////// 쉐도우맵
	void RenderShadowMap();
	const UINT m_DsvDescriptorsCount{ 2 };
	D3D12_CPU_DESCRIPTOR_HANDLE m_ShadowMapCPUHandle;
	ID3D12Resource*			m_pShadowMap;
	////////////////////////////// 쉐도우맵


	//// GBuffer 
	static const UINT m_GBuffersCount{ 3 };

	ID3D12Resource*				m_GBuffers[m_GBuffersCount];
	D3D12_CPU_DESCRIPTOR_HANDLE m_GBufferCPUHandle[m_GBuffersCount];
	ID3D12DescriptorHeap* m_GBufferHeap;
	UINT m_GBufferDescriptorSize;

	//CTriangleShader m_RedShader;
	//CGreenShader m_GreenShader;
	//CBlueShader m_BlueShader;
	//CRenderComputeShader m_RenderComputeShader;

	// Texture*		m_pGBufferTexture;
	float	m_GBufferClearValue[3][4]{
		{ 1.f, 0.f, 0.f, 1.f },
		{ 0.f, 1.f, 0.f, 1.f },
		{ 0.f, 0.f, 1.f, 1.f }
	};

	///////////////////// 컴퓨트 쉐이더를 위한 변수
	float	m_RWClearValue[4] = { 1.f, 0.f, 1.f, 0.f };
	ID3D12Resource* m_ComputeRWResource; // 작성함 

	const UINT m_UAVParameterIndex = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE m_UAVGPUDescriptorHandle;

private:  
	//스왑 체인, 디바이스, 서술자 힙, 명령 큐/할당자/리스트를 생성하는 함수이다.
	// OnCreate() 내부에서 사용한다.
	void CreateRWResourceViews() ;
	void CreateMRTComputeShader() ;
	
	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateGBufferView(); // MRT를 위한 버퍼
	void CreateCommandQueueAndList();

	//CPU와 GPU를 동기화하는 함수이다.
	void WaitForGpuComplete();
	void MoveToNextFrame();

	//렌더링할 메쉬와 게임 객체를 생성하고 소멸하는 함수이다.
	void BuildObjects();
	void ReleaseObjects();

	//윈도우 클라이언트 영역의 크기가 바뀌면 실행하는 함수.
	void OnResizeBackBuffers();

	// Render() 내부에서 사용하는 함수들이다.	
	void RenderGBuffers(); // GBuffer에 Render
	void Blur(); // 계산쉐이더를 통해 블러링한다.
	void RenderSwapChain(); // SwapChain에 Render

	// 키보드와 마우스 메시지를 처리하는 부분이다.
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	void RenderOnGbuffer();
	void RenderOnCompute();
	void RenderOnSwapchain();

	void BuildShaders();
	void BuildTESTObjects();

public:
	CGameFramework();
	~CGameFramework();

	//프레임워크를 초기화하는 함수이다(주 윈도우가 생성되면 호출된다).
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	//프레임워크의 핵심(사용자 입력, 애니메이션, 업데이트)을 구성하는 함수이다. 
	void UpdateGamelogic(float );

	// 렌더링을 처리하는 함수이다.
	void Render();

	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다.
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
private: 
	short m_MouseWheelData{ 0 }; // 120: 휠을 올렸을때, -120: 휠을 내렸을때, 한번 휠을 올리거나 내리면 그 상태로 계속 저장되므로 0으로 다시 만들어줘야함
	 
	Scene *m_pScene{ nullptr };
	
	// 블러를 위한 컴퓨트
	ComputeShader* m_horizenShader{ nullptr };
	ComputeShader* m_verticalShader{ nullptr };

	MyDescriptorHeap* m_TESTHeap_1{ nullptr };
};

