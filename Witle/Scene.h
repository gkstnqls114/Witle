#pragma once 

class ParameterForm;

class Scene
{
protected:
	//그래픽 루트 시그너쳐를 생성한다.
	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) = 0;

public:
	Scene();
	virtual ~Scene();

	void CreateRootSignature(ID3D12Device *pd3dDevice);

	//씬에서 마우스와 키보드 메시지를 처리한다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) = 0;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void ReleaseObjects() = 0;

	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) = 0; 
	
	virtual void AnimateObjects(float fTimeElapsed) = 0;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) = 0;

	virtual void ReleaseUploadBuffers() = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdateGamelogic, LastUpdate 순으로 호출되는 함수이다. 
	virtual void Update(float ElapsedTime) = 0;
	virtual void LastUpdate(float ElapsedTime) = 0;

	ID3D12RootSignature* GetGraphicsRootSignature() const { return m_pd3dGraphicsRootSignature; }

protected:

	ID3D12RootSignature* m_pd3dGraphicsRootSignature{ nullptr }; // 루트 시그니처
	
};