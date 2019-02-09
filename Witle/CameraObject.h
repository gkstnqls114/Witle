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
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override {};

public:
	CameraObject(const std::string& entityID);
	virtual ~CameraObject();

	virtual void Update(float fElapsedTime) override {}; // Camera�� ������Ʈ�� ���������ʴ´�.
	void LastUpdate(float fElapsedTime); // Update ��� LastUpdate�� ����Ѵ�.
	
	void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList);

	void ChangeCamera(Camera* pNewCamera);

	Camera* GetCamera() const { return m_pCameraComponent; }
	MyFrustum* GetFrustum() const { return m_pFrustum; }
};