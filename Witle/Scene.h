#pragma once 
 
class Scene
{ 
public: 
	// 마우스 메시지를 처리합니다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	// 키보드 메시지를 처리합니다.
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) = 0;

	// 필요한 오브젝트들을 빌드합니다.
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;

	virtual void ReleaseUploadBuffers() = 0;
	virtual void ReleaseObjects() = 0;

	// 화면에 그림을 그리는 함수.
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) = 0;
	
	//// UpdateGamelogic ///////////////////////////////////

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void UpdatePhysics(float ElapsedTime) = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void Update(float ElapsedTime) = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 
	virtual void AnimateObjects(float fTimeElapsed) = 0;

	// GameFramework의 UpdateGamelogic 함수 내부에서 UpdatePhysics, AnimateObjects, LastUpdate 순으로 호출되는 함수. 

	virtual void LastUpdate(float ElapsedTime) = 0;
	//// UpdateGamelogic ///////////////////////////////////

public:
	Scene();
	virtual ~Scene();
	 
};