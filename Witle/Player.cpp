#include "stdafx.h"
#include "Transform.h"
#include "GameTimer.h"
#include "FollowCam.h"
#include "Terrain.h"
#include "Player.h"

void Player::OnPlayerUpdateCallback(float fTimeElapsed, Camera* pCamera)
{
	if (!m_pPlayerUpdatedContext) return;

	Terrain *pTerrain = (Terrain *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = m_Transform->GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 6.0f;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		m_xmf3Velocity = xmf3PlayerVelocity;
		xmf3PlayerPosition.y = fHeight;
		m_Transform->SetPosition(xmf3PlayerPosition);
	}
}

void Player::OnCameraUpdateCallback(float fTimeElapsed, Camera* pCamera)
{
	if (!m_pCameraUpdatedContext) return;
	
	Terrain *pTerrain = (Terrain *)m_pCameraUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 5.0f;
	if (xmf3CameraPosition.y <= fHeight)
	{
		xmf3CameraPosition.y = fHeight;
		pCamera->SetPosition(xmf3CameraPosition); 
		static_cast<FollowCam *>(pCamera)->SetLookAt(m_Transform->GetPosition());
	}
}

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
	 
	// �̵����� ����Ѵ�.
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Gravity, fTimeElapsed, false)); // �߷°��
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

	m_Transform->Move(m_xmf3Velocity); // �̵�����ŭ �����δ�.
	pCamera->Move(m_xmf3Velocity);

	// �÷��̾� �ݹ�
	OnPlayerUpdateCallback(fTimeElapsed, pCamera);

	// ī�޶� ���������� �̵�
	pCamera->Update(fTimeElapsed, m_Transform->GetPosition());
	static_cast<FollowCam*>(pCamera)->Update(fTimeElapsed, m_Transform->GetPosition());
	OnCameraUpdateCallback(fTimeElapsed, pCamera); 
	static_cast<FollowCam*>(pCamera)->RegenerateViewMatrix();

	// �̵��� �پ���.
	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed); // ���ҷ�
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

void Player::VelocityMove(const XMFLOAT3 & vMove)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, vMove);
}
