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
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_pPlayerMovement->AlreadyUpdate(fTimeElapsed));
	BoundingOrientedBox AlreadyBBox = m_pMyBOBox->GetBOBox();
	AlreadyBBox.Center = AlreadyPosition;
	return AlreadyBBox;
}
XMFLOAT3 Player::CalculateAlreadyPosition(float fTimeElapsed)
{
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_pPlayerMovement->AlreadyUpdate(fTimeElapsed));
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
	m_PlayerModel = LoadObject::LoadGeometryAndAnimationFromFile_forPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Chracter.bin", NULL);
	m_pLoadObject = m_PlayerModel->m_pModelRootObject;
	 
	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_PlayerModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	XMFLOAT3 center{ 0.f, 75.f, 0.f };
	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_pMyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
	 
	m_pPlayerStatus = new PlayerStatus(this, pd3dDevice, pd3dCommandList);
	m_pPlayerMovement = new PlayerMovement(this);
	 
	m_pBroom = new Broom(m_pPlayerMovement);

	m_Transform.SetPosition(0.f, 75.f ,0.f);
	
	SetUpdatedContext(pContext); 
}

Player::~Player()
{ 
	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;
}
 

void Player::ReleaseMembers()
{
	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;
	if (m_pBroom)
	{ 
		m_pBroom->ReleaseObjects();
		delete m_pBroom;
		m_pBroom = nullptr;
	}
	if (m_pLoadObject)
	{
		m_pLoadObject->ReleaseObjects(); 
		m_pLoadObject = nullptr;
	}
	if (m_PlayerModel)
	{
		//m_PlayerModel->ReleaseObjects();
		//delete m_PlayerModel;
		//m_PlayerModel = nullptr;
	}
	if (m_pMyBOBox)
	{
		m_pMyBOBox->ReleaseObjects();
		delete m_pMyBOBox;
		m_pMyBOBox = nullptr;
	}
	if (m_pPlayerStatus)
	{
		m_pPlayerStatus->ReleaseObjects();
		delete m_pPlayerStatus;
		m_pPlayerStatus = nullptr;
	}
	if (m_pPlayerMovement)
	{
		m_pPlayerMovement->ReleaseObjects();
		delete m_pPlayerMovement;
		m_pPlayerMovement = nullptr;
	}
}

void Player::ReleaseMemberUploadBuffers()
{
	if (m_pLoadObject) m_pLoadObject->ReleaseUploadBuffers();
	if (m_PlayerModel)m_PlayerModel->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}

void Player::Update(float fElapsedTime)
{ 
	m_pBroom->Update(fElapsedTime);

	// �̵����� ����Ѵ�. 
	m_pPlayerMovement->Update(fElapsedTime);
	
	// �̵�����ŭ �����δ�. 
	 Move(Vector3::ScalarProduct(m_pPlayerMovement->m_xmf3Velocity, fElapsedTime, false));
	 
	 
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
	 
	m_pLoadObject->Animate(fElapsedTime);
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
	m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_IDLE.ID);

	if (dwDirection & DIR_FORWARD)	m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_FORWARD.ID);
	if (dwDirection & DIR_BACKWARD) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_BACKWARD.ID);
	if (dwDirection & DIR_RIGHT) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_RIGHT.ID);
	if (dwDirection & DIR_LEFT) m_pLoadObject->SetTrackAnimationSet(0, ANIMATION_LEFT.ID); 
}

void Player::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
	m_pMyBOBox->Move(xmf3Shift);
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
		//�÷��̾��� �̵��� ���͸� xmf3Shift ���͸�ŭ ���Ѵ�. 
		m_pPlayerMovement->MoveVelocity(dwDirection, fTimeElapsed);
	}
	else
	{
		m_pPlayerMovement->ReduceVelocity(fTimeElapsed);
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
