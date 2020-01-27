#pragma once 
 
class Scene
{ 
public: 
	// ���콺 �޽����� ó���մϴ�.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	// Ű���� �޽����� ó���մϴ�.
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	// �ʿ��� ������Ʈ���� �����մϴ�.
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;

	virtual void ReleaseUploadBuffers() = 0;
	virtual void ReleaseObjects() = 0;

	// ȭ�鿡 �׸��� �׸��� �Լ�.
	
	// ������/���۵� �������� isGBuffer�� ���� �����մϴ�.
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) = 0;

	// �׸��ڸ� �������ϱ� ���� �Լ��Դϴ�.
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) = 0;

	// �÷��̾� �׸��ڸ� �������ϱ� ���� �Լ��Դϴ�.
	virtual void RenderForPlayerShadow(ID3D12GraphicsCommandList *pd3dCommandList) = 0;

	//// UpdateGamelogic /////////////////////////////////// 
	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual bool ProcessInput(HWND hWnd) = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void UpdatePhysics() = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void Update() = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�. 
	virtual void AnimateObjects() = 0;

	// GameFramework�� UpdateGamelogic �Լ� ���ο��� UpdatePhysics, AnimateObjects, LastUpdate ������ ȣ��Ǵ� �Լ�.  
	virtual void LastUpdate() = 0; 
	//// UpdateGamelogic ///////////////////////////////////

	// Render ���� ������Ʈ �ؾ��� ��� ȣ��Ǵ� �Լ�
	virtual void AfterRender() = 0;

public:
	Scene();
	virtual ~Scene();
	 
};