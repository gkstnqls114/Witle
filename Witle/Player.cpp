#include "stdafx.h"
#include "Transform.h"
#include "GameTimer.h"
#include "Camera.h"
#include "Player.h"

Player::Player(const std::string& entityID) 
	: GameObject(entityID)
{  
	 
	// m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
	// m_pCamera->SetTimeLag(0.25f);
	// m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, -50.0f));
	// m_pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
}

Player::~Player()
{
}

void Player::Update()
{ 
}

void Player::Update(Camera * pCamera)
{
	float fTimeElapsed = CGameTimer::GetInstance()->GetTimeElapsed();
	 
	// 이동량을 계산한다.
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Gravity, fTimeElapsed, false)); // 중력계산
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.y * m_xmf3Velocity.y + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	m_Transform->Move(m_xmf3Velocity); // 이동량만큼 움직인다.
	pCamera->Move(m_xmf3Velocity);

	// 카메라도 마찬가지로 이동
	pCamera->Update(fTimeElapsed, m_Transform->GetPosition());

	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed); // 감소량
	if (fDeceleration > fLength) fDeceleration = fLength;
	// 이동량 줄어든다.
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

void Player::VelocityMove(const XMFLOAT3 & vMove)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, vMove);
}
