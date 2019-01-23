#pragma once
#include "GameObject.h"

class Camera;

class CameraObject
	:public GameObject
{
	Camera* m_pCameraComponent;

public:
	CameraObject(const std::string& entityID);
	virtual ~CameraObject();

	virtual void Update(float fElapsedTime) {}; // Camera�� ������Ʈ�� ���������ʴ´�.
	void LastUpdate(float fElapsedTime); // Update ��� LastUpdate�� ����Ѵ�.
	
	void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList);
	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseShaderVariables();

	void ChangeCamera(Camera* pNewCamera);

	Camera* GetCamera() const { return m_pCameraComponent; }
};