#pragma once
#include "Skill.h"

class MyBOBox;
class Camera;
class CameraObject;
class Player;

// ���������� ���� ī�޶� ������Ʈ�� �����Ѵ�.
class Sniping :
	public Skill
{ 
	Player* m_pPlayer{ nullptr };
	CameraObject* m_pCameraObject{ nullptr };

	// �⺻ ��� 
	Camera* m_pBaseCameraComponent{ nullptr };
	MyBOBox*		   m_BaseAttackBOBox{ nullptr };

	// �������� ��� 
	Camera* m_pSnipingCameraComponent{ nullptr };
	MyBOBox*		   m_SnipingAttackBOBox{ nullptr };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Sniping(CameraObject* pCamera, Player* pPlayer, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Sniping();

	Camera* GetBaseCamera() const
	{  
		return m_pBaseCameraComponent;
	}

	Camera* GetSnipingCamera() const
	{ 
		return m_pSnipingCameraComponent; 
	}
	
	void Rotate(float x, float y, float z);
	void Move(float x, float y, float z);
	void Render(ID3D12GraphicsCommandList * pd3dCommandList);

public:
	virtual void DoNotUse() override;
	virtual void DoUse() override;
};

