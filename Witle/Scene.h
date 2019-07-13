#pragma once 
 
class Scene
{ 
public: 
	// ���콺 �޽����� ó���մϴ�.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	// Ű���� �޽����� ó���մϴ�.
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) = 0;

	// �ʿ��� ������Ʈ���� �����մϴ�.
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;

	virtual void ReleaseUploadBuffers() = 0;
	virtual void ReleaseObjects() = 0;

	// ȭ�鿡 �׸��� �׸��� �Լ�.
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) = 0;
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) = 0;
	
	//// UpdateGamelogic ///////////////////////////////////

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void UpdatePhysics(float ElapsedTime) = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void Update(float ElapsedTime) = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void AnimateObjects(float fTimeElapsed) = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 

	virtual void LastUpdate(float ElapsedTime) = 0;
	//// UpdateGamelogic ///////////////////////////////////

public:
	Scene();
	virtual ~Scene();
	 
};