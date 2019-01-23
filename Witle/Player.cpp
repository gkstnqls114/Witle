#include "stdafx.h"
#include "Transform.h"
#include "GameTimer.h"
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
	float fTimeElapsed = CGameTimer::GetInstance()->GetTimeElapsed();

	if (m_xmf3Velocity.x <= 0.0f && m_xmf3Velocity.y <= 0.0f && m_xmf3Velocity.z <= 0.0f)
	{
		float fLength = Vector3::Length(m_xmf3Velocity);
		float fDeceleration = (m_fFriction * fTimeElapsed); // ���ҷ�
		if (fDeceleration > fLength) fDeceleration = fLength;
		// �̵��� �پ���.
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
	}

	// �̵����� ����Ѵ�.
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Gravity, fTimeElapsed, false));
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	m_Transform->Move(m_xmf3Velocity); // �̵�����ŭ �����δ�.
}

void Player::VelocityMove(const XMFLOAT3 & vMove)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, vMove);
}
