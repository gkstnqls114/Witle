#pragma once
#include "Skill.h"

class Camera;
class CameraObject;
class Player;

// ���������� ���� ī�޶� ������Ʈ�� �����Ѵ�.
class Sniping :
	public Skill
{
	Player* m_pPlayer{ nullptr };
	CameraObject* m_pCameraObject{ nullptr };
	Camera* m_pBaseCameraComponent{ nullptr };
	Camera* m_pSnipingCameraComponent{ nullptr };

public:
	Sniping(CameraObject* pCamera, Player* pPlayer, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Sniping();

	Camera* GetBaseCamera() const { return m_pBaseCameraComponent; }
	Camera* GetSnipingCamera() const { return m_pSnipingCameraComponent; }


public:
	virtual void DoNotUse() override;
	virtual void DoUse() override;
};

