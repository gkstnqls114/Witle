#pragma once 
 
class Scene
{ 
public: 
	// 마우스 메시지를 처리합니다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	// 키보드 메시지를 처리합니다.
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	// 필요한 오브젝트들을 빌드합니다.
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;

	virtual void ReleaseUploadBuffers() = 0;
	virtual void ReleaseObjects() = 0;

	// 화면에 그림을 그리는 함수.
	
	// 포워드/디퍼드 렌더링을 isGBuffer를 통해 구분합니다.
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) = 0;

	// 그림자를 렌더링하기 위한 함수입니다.
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) = 0;

	// 플레이어 그림자를 렌더링하기 위한 함수입니다.
	virtual void RenderForPlayerShadow(ID3D12GraphicsCommandList *pd3dCommandList) = 0;

	//// UpdateGamelogic ///////////////////////////////////

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual bool ProcessInput(HWND hWnd) = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void UpdatePhysics() = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void Update() = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void AnimateObjects() = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 

	virtual void LastUpdate() = 0;
	//// UpdateGamelogic ///////////////////////////////////

public:
	Scene();
	virtual ~Scene();
	 
};