#include "stdafx.h"
#include "PlayerMovement.h"
#include "MyBOBox.h" 
#include "GameInput.h"
#include "Shader.h"
#include "Object.h"
#include "ShaderManager.h" 
#include "Transform.h"
#include "PlayerStatus.h"
#include "GameTimer.h"
#include "FollowCam.h"
#include "Broom.h"
#include "Terrain.h"
#include "Player.h"

#define ANIMATION_IDLE 0
#define ANIMATION_FORWARD 1
#define ANIMATION_BACKWARD 2
#define ANIMATION_LEFT 3
#define ANIMATION_RIGHT 4

void Player::OnPlayerUpdateCallback(float fTimeElapsed)
{
	if (!m_pPlayerUpdatedContext) return;

	Terrain *pTerrain = (Terrain *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = m_Transform.GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad);
	fHeight += 1.f;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		m_pPlayerMovement->m_xmf3Velocity = xmf3PlayerVelocity;
		xmf3PlayerPosition.y = fHeight;
		m_Transform.SetPosition(xmf3PlayerPosition);
	}
}
XMFLOAT3 Player::CalculateAlreadyVelocity(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_pPlayerMovement->m_xmf3Velocity, m_pPlayerMovement->m_xmf3Gravity);
	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
	float fMaxVelocityXZ = m_pPlayerMovement->m_fMaxVelocityXZ;
	if (fLength > m_pPlayerMovement->m_fMaxVelocityXZ)
	{
		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_pPlayerMovement->m_fMaxVelocityY;
	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
	if (fLength > m_pPlayerMovement->m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);

	return xmf3Velocity;
}
BoundingOrientedBox Player::CalculateAlreadyBoundingBox(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = CalculateAlreadyVelocity(fTimeElapsed);
	BoundingOrientedBox AlreadyBBox = m_pMyBOBox->GetBOBox();
	AlreadyBBox.Center = Vector3::Add(AlreadyBBox.Center, AlreadyVelocity);
	return AlreadyBBox;
}
XMFLOAT3 Player::CalculateAlreadyPosition(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = CalculateAlreadyVelocity(fTimeElapsed);
	XMFLOAT3 AlreadyPosition = m_Transform.GetPosition();
	AlreadyPosition = Vector3::Add(AlreadyPosition, AlreadyVelocity);
	return AlreadyPosition;
}

//void Player::OnCameraUpdateCallback(float fTimeElapsed, Camera* pCamera)
//{
//	if (!m_pCameraUpdatedContext) return;
//	
//	Terrain *pTerrain = (Terrain *)m_pCameraUpdatedContext;
//	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
//	XMFLOAT3 xmf3CameraPosition = pCamera->GetOwner()->GetTransform().GetPosition();
//	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
//	bool bReverseQuad = ((z % 2) != 0);
//	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad);
//	fHeight = fHeight + 5.f;
//	if (xmf3CameraPosition.y <= fHeight)
//	{
//		xmf3CameraPosition.y = fHeight;
//		pCamera->GetOwner()->GetTransform().SetPosition(xmf3CameraPosition);
//		static_cast<FollowCam *>(pCamera)->SetLookAt(m_Transform.GetPosition());
//	}
//}
 

Player::Player(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
	: GameObject(entityID)
{ 
	m_PlayerModel = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Angrybot.bin", NULL);
	m_pLoadObject = m_PlayerModel->m_pModelRootObject;
	 
	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_PlayerModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	XMFLOAT3 center{ 0.f, 75.f, 0.f };
	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_pMyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
	 
	m_pPlayerStatus = new PlayerStatus(this, pd3dDevice, pd3dCommandList);
	m_pPlayerMovement = new PlayerMovement(this);
	 
	m_pBroom = new Broom(m_pPlayerMovement);

	SetUpdatedContext(pContext); 
}

Player::~Player()
{
	ReleaseMembers();
	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;
}
 

void Player::ReleaseMembers()
{
	if (m_PlayerModel)
	{
		m_PlayerModel->ReleaseObjects();
		delete m_PlayerModel;
		m_PlayerModel = nullptr;
	}
	if (m_pMyBOBox)
	{
		delete m_pMyBOBox;
		m_pMyBOBox = nullptr;
	}
	if (m_pPlayerStatus)
	{
		delete m_pPlayerStatus;
		m_pPlayerStatus = nullptr;
	}
	if (m_pPlayerMovement)
	{
		delete m_pPlayerMovement;
		m_pPlayerMovement = nullptr;
	}
}

void Player::ReleaseMemberUploadBuffers()
{
	if (m_pLoadObject) m_pLoadObject->ReleaseUploadBuffers();
	
}

void Player::Update(float fElapsedTime)
{ 
	m_pBroom->Update(fElapsedTime);

	// �̵����� ����Ѵ�. 
	m_pPlayerMovement->Update(fElapsedTime);
	
	// �̵�����ŭ �����δ�.
	if (m_pBroom->GetisUsing())
	{
		Move(Vector3::ScalarProduct(m_pPlayerMovement->m_xmf3Velocity, fElapsedTime, false));
	}
	 
	// �÷��̾� �ݹ�
	// OnPlayerUpdateCallback(fElapsedTime);
	 
	// �̵��� �پ���.
	//fLength = Vector3::Length(m_xmf3Velocity);
	//float fDeceleration = (m_fFriction * fElapsedTime); // ���ҷ�
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

	//m_pLoadObject->SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1); // ���࿡ fLength�� 0�� �ƴ϶��... �� �����δٸ�...
}

void Player::SubstractHP(int sub)
{ 
	m_pPlayerStatus->m_HP -= sub; 
}

void Player::Animate(float fElapsedTime)
{
	// �ݵ�� Ʈ������ ������Ʈ..! 
	m_Transform.Update(fElapsedTime);

	//// ��ġ�� �ȸ¾Ƽ� ������  
	float fPitch = -90.f;
	float fYaw = 0.f;
	float fRoll = 0.f;
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_pLoadObject->m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_Transform.GetpWorldMatrixs());
	 
	// m_pLoadObject->Animate(fElapsedTime);
}

void Player::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	if (!m_isRendering) return;

#ifdef _SHOW_BOUNDINGBOX
	m_pMyBOBox->Render(pd3dCommandList, m_Transform.GetpWorldMatrixs());
#endif // _SHOW_BOUNDINGBOX

	m_pLoadObject->Render(pd3dCommandList);
	// m_pPlayerStatus->Render(pd3dCommandList);
}
 
void Player::SetTrackAnimationSet(ULONG dwDirection)
{ 
	m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_IDLE);

	if (dwDirection & DIR_FORWARD)	m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_FORWARD);
	if (dwDirection & DIR_BACKWARD) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_BACKWARD);
	if (dwDirection & DIR_RIGHT) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_RIGHT);
	if (dwDirection & DIR_LEFT) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_LEFT); 
}

void Player::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
	m_pMyBOBox->Move(xmf3Shift);
}

void Player::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_pPlayerMovement->m_xmf3Velocity = Vector3::Add(m_pPlayerMovement->m_xmf3Velocity, xmf3Shift);
}

void Player::Rotate(float x, float y, float z)
{
	m_Transform.Rotate(x, y, z);
 	m_pMyBOBox->Rotate(m_pPlayerMovement->m_fRoll, m_pPlayerMovement->m_fYaw, m_pPlayerMovement->m_fPitch);
}

void Player::ProcessInput(float fTimeElapsed)
{
	DWORD dwDirection = 0;
	 
	if (GameInput::IsKeydownW()) dwDirection |= DIR_FORWARD;
	if (GameInput::IsKeydownS()) dwDirection |= DIR_BACKWARD;
	if (GameInput::IsKeydownA()) dwDirection |= DIR_LEFT;
	if (GameInput::IsKeydownD()) dwDirection |= DIR_RIGHT;
	 
	// ���� Ű���� �����¿� �����δٸ�...
	if (dwDirection != 0)
	{ 
		AXIS axis = AXIS{ m_Transform.GetCoorAxis() };

		XMFLOAT3 xmf3Shift = XMFLOAT3(0.f, 0.f, 0.f); // �̵���

		/*�÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�). �̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�.
		�÷��̾��� �̵� �ӷ��� (20m/��)�� �����Ѵ�.*/
		float fDistance = m_pPlayerMovement->m_fDistance * fTimeElapsed; // 1�ʴ� �ִ� �ӷ� 20m���� ����, ���� 1 = 1cm

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -fDistance);
		
		//�÷��̾��� �̵��� ���͸� xmf3Shift ���͸�ŭ ���Ѵ�. 
		if (m_pBroom->GetisUsing())
		{
			MoveVelocity(xmf3Shift);
		}
		else
		{
			Move(xmf3Shift);
		}
	}
	else
	{
		XMFLOAT3 Veclocity = GetVelocity();
		if (Vector3::Length(Veclocity) > 0.f)
		{
			float fLength = Vector3::Length(Veclocity);
			float fDeceleration = (m_pPlayerMovement->m_fFriction * fTimeElapsed); //�ش����� Friction
			if (fDeceleration > fLength) fDeceleration = fLength;
			MoveVelocity(Vector3::ScalarProduct(Veclocity, -fDeceleration, true));
		}
	}

	SetTrackAnimationSet(dwDirection);
}

void Player::UseSkill_Broom()
{
	m_pBroom->DoUse();
}

XMFLOAT3 Player::GetVelocity() const
{
	return m_pPlayerMovement->m_xmf3Velocity;
}

void Player::SetVelocity(const XMFLOAT3 & velocity)
{
	m_pPlayerMovement->m_xmf3Velocity = velocity;
}
