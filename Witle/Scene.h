#pragma once 

class CScene
{
protected:
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	virtual ComPtr<ID3D12RootSignature> CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) = 0;

	virtual void Update(float ElapsedTime) = 0;

public:
	CScene();
	virtual ~CScene();

	//������ ���콺�� Ű���� �޽����� ó���Ѵ�.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) = 0;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void ReleaseObjects() = 0;

	virtual bool ProcessInput(HWND hWnd, POINT OldCursor, float ElapsedTime) = 0;
	virtual bool ProcessMouseWheel(HWND hWnd, short WheelData, float ElapsedTime) = 0;
	
	virtual void AnimateObjects(float fTimeElapsed) = 0;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) = 0;
	virtual void OnprepareRender() = 0;

	virtual void ReleaseUploadBuffers() = 0;
	
	ID3D12RootSignature* GetGraphicsRootSignature() const { return m_pd3dGraphicsRootSignature.Get(); }

protected:

	ComPtr<ID3D12RootSignature> m_pd3dGraphicsRootSignature{ nullptr }; // ��Ʈ �ñ״�ó
	ComPtr<ID3D12PipelineState> m_pd3dPipelineState{ nullptr }; //PSO

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_ScissorRect;
};