#pragma once
#include "Skill.h"

class Camera;
class CameraObject;
class Player;

// 스나이핑을 위한 카메라 컴포넌트를 관리한다.
class Sniping :
	public Skill
{
	Player* m_pPlayer{ nullptr };
	CameraObject* m_pCameraObject{ nullptr };

	bool m_isConnectBase{ false };
	bool m_isConnectSniping{ false };
	Camera* m_pBaseCameraComponent{ nullptr };
	Camera* m_pSnipingCameraComponent{ nullptr };

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


public:
	virtual void DoNotUse() override;
	virtual void DoUse() override;
};

