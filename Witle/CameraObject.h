#pragma once
#include "GameObject.h"

class Camera;

class CameraObject
	:public GameObject
{
	Camera* m_CameraComponent;

public:
	CameraObject(const std::string& entityID);
	virtual ~CameraObject();

	virtual void Update(float fElapsedTime) {}; // Camera�� ������Ʈ�� ���������ʴ´�.
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) override;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) override;
	virtual void ReleaseShaderVariables() override;

	void LastUpdate(float fElapsedTime) {}; // Update ��� LastUpdate�� ����Ѵ�.

	void ChangeCamera(Camera* pNewCamera);

	Camera* GetCamera() const { return m_CameraComponent; } 
};