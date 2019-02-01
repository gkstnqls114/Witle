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
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ��������� �����Ѵ�.
	virtual void ReleaseMembers();

public:
	CameraObject(const std::string& entityID);
	virtual ~CameraObject();

	virtual void Update(float fElapsedTime) override {}; // Camera�� ������Ʈ�� ���������ʴ´�.
	void LastUpdate(float fElapsedTime); // Update ��� LastUpdate�� ����Ѵ�.
	
	void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList);
	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseShaderVariables();

	void ChangeCamera(Camera* pNewCamera);

	Camera* GetCamera() const { return m_pCameraComponent; }
};