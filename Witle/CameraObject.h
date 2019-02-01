#pragma once
#include "GameObject.h"

class Camera;
class MyFrustum;

class CameraObject
	:public GameObject
{
	Camera*      m_pCameraComponent{ nullptr };
	MyFrustum*   m_pFrustum{ nullptr };

protected:
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 멤버변수를 해제한다.
	virtual void ReleaseMembers();

public:
	CameraObject(const std::string& entityID);
	virtual ~CameraObject();

	virtual void Update(float fElapsedTime) override {}; // Camera는 업데이트를 수행하지않는다.
	void LastUpdate(float fElapsedTime); // Update 대신 LastUpdate를 사용한다.
	
	void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList);
	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseShaderVariables();

	void ChangeCamera(Camera* pNewCamera);

	Camera* GetCamera() const { return m_pCameraComponent; }
};