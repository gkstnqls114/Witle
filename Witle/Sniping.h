#pragma once
#include "Skill.h"

class MyBOBox;
class Camera;
class CameraObject;
class Player;

// 스나이핑을 위한 카메라 컴포넌트를 관리한다.
class Sniping :
	public Skill
{ 
	Player* m_pPlayer{ nullptr };
	CameraObject* m_pCameraObject{ nullptr };

	// 기본 모드 
	Camera* m_pBaseCameraComponent{ nullptr };
	MyBOBox*		   m_BaseAttackBOBox{ nullptr };

	// 스나이핑 모드 
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

