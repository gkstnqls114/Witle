#pragma once
#include "GameObject.h"

class Camera;

// CHeightMapTerrain 입니다.
class Player :
	public GameObject
{
	//XMFLOAT3 m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3 m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//XMFLOAT3 m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//XMFLOAT3 m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f); //현재 중력은 존재하지않는다.
	float m_fMaxVelocityXZ = 300.0f;
	float m_fMaxVelocityY = 400.0f;
	float m_fFriction = 25.0f;

	// 회전량
	float m_fPitch = 0.0f;
	float m_fRoll = 0.0f;
	float m_fYaw = 0.0f;

public:
	Player(const std::string& entityID);
	virtual ~Player();

	virtual void Update() override;
	void Update(Camera* camera);

	void VelocityMove(const XMFLOAT3& vMove);
	XMFLOAT3 GetVelocity() const { return m_xmf3Velocity; };
};