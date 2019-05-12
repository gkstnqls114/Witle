#include "stdafx.h"
#include "PlayerMovement.h"
#include "MyBOBox.h" 
#include "GameInput.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "Sniping.h"
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
 
static float playerOffestX = 0.f;
static float playerOffestY = 75.f;
static float playerOffestZ = 50.f;

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
	m_pHaep = new MyDescriptorHeap();
	m_pHaep->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2, 0);

	m_pTexture_Cloth = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture_Cloth->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Character_cloth_D.dds", 0);
	m_pTexture_Body = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture_Body->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Character_body_D.dds", 0);

	m_pHaep->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture_Cloth, ROOTPARAMETER_TEXTURE, false, 0);
	m_pHaep->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture_Body, ROOTPARAMETER_TEXTURE, false, 1);

	m_PlayerModel_Cloth = LoadObject::LoadGeometryAndAnimationFromFile_forPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Character_cloth.bin", NULL);
	m_PlayerModel_Body = LoadObject::LoadGeometryAndAnimationFromFile_forPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Character_body.bin", NULL);
	m_pLoadObject_Cloth = m_PlayerModel_Cloth->m_pModelRootObject;
	m_pLoadObject_Body = m_PlayerModel_Body->m_pModelRootObject;
	 
	m_pLoadObject_Cloth->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_PlayerModel_Cloth);
	m_pLoadObject_Cloth->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	m_pLoadObject_Body->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_PlayerModel_Body);
	m_pLoadObject_Body->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	 
	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_pMyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);
	
	m_pPlayerStatus = new PlayerStatus(this, pd3dDevice, pd3dCommandList);
	m_pPlayerMovement = new PlayerMovement(this);
	 
	m_pBroom = new Broom(m_pPlayerMovement);

	m_Transform.SetPosition(100.f, 57.f, 100.f);// 캐릭터가 중앙에 있지않아서 어쩔수없이 설정;

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
	if (m_pTexture_Cloth)
	{
		m_pTexture_Cloth->ReleaseObjects();
		delete m_pTexture_Cloth;
		m_pTexture_Cloth = nullptr;
	}
	if (m_pTexture_Body)
	{
		m_pTexture_Body->ReleaseObjects();
		delete m_pTexture_Body;
		m_pTexture_Body = nullptr;
	}
	if (m_pBroom)
	{ 
		m_pBroom->ReleaseObjects();
		delete m_pBroom;
		m_pBroom = nullptr;
	}
	if (m_pLoadObject_Cloth)
	{
		m_pLoadObject_Cloth->ReleaseObjects();
		m_pLoadObject_Cloth = nullptr;
	}

	if (m_pLoadObject_Body)
	{
		m_pLoadObject_Body->ReleaseObjects();
		m_pLoadObject_Body = nullptr;
	}
	if (m_PlayerModel_Cloth)
	{
		m_PlayerModel_Cloth->ReleaseObjects();
		delete m_PlayerModel_Cloth;
		m_PlayerModel_Cloth = nullptr;
	} 
	if (m_PlayerModel_Body)
	{
		m_PlayerModel_Body->ReleaseObjects();
		delete m_PlayerModel_Body;
		m_PlayerModel_Body = nullptr;
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
	if (m_pTexture_Cloth) m_pTexture_Cloth->ReleaseUploadBuffers(); 
	if (m_pTexture_Body) m_pTexture_Body->ReleaseUploadBuffers(); 
	if (m_pLoadObject_Cloth) m_pLoadObject_Cloth->ReleaseUploadBuffers();
	if (m_pLoadObject_Body) m_pLoadObject_Body->ReleaseUploadBuffers();
	if (m_PlayerModel_Cloth)m_PlayerModel_Cloth->ReleaseUploadBuffers();
	if (m_PlayerModel_Body)m_PlayerModel_Body->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}

void Player::Update(float fElapsedTime)
{ 
	m_pBroom->Update(fElapsedTime);

	// 이동량을 계산한다. 
	m_pPlayerMovement->Update(fElapsedTime);
	
	// 이동량만큼 움직인다. 
	Move(Vector3::ScalarProduct(m_pPlayerMovement->m_xmf3Velocity, fElapsedTime, false));

	m_pMyBOBox->SetPosition(
		XMFLOAT3(
		m_Transform.GetPosition().x, 
		75.f,
		m_Transform.GetPosition().z)
	);
	
	// 플레이어 콜백
	// OnPlayerUpdateCallback(fElapsedTime);
	 
	 
	// 이동량 줄어든다.
	//fLength = Vector3::Length(m_xmf3Velocity);
	//float fDeceleration = (m_fFriction * fElapsedTime); // 감소량
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

	//m_pLoadObject->SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1); // 만약에 fLength가 0이 아니라면... 즉 움직인다면...
}

void Player::SubstractHP(int sub)
{ 
	m_pPlayerStatus->m_HP -= sub; 
	std::cout << m_pPlayerStatus->m_HP << std::endl;
}

void Player::Animate(float fElapsedTime)
{
	// animate 이전에 현재 설정된 애니메이션 수행하도록 설정
	SetTrackAnimationSet();

	// 반드시 트랜스폼 업데이트..! 
	m_Transform.Update(fElapsedTime);

	//// 위치가 안맞아서 재조정  
	float fPitch = -90.f;
	float fYaw = 0.f;
	float fRoll = 0.f;
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_pLoadObject_Cloth->m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_Transform.GetpWorldMatrixs());
	m_pLoadObject_Body->m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_Transform.GetpWorldMatrixs());
	 
	m_pLoadObject_Cloth->Animate(fElapsedTime);
	m_pLoadObject_Body->Animate(fElapsedTime);
}

void Player::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	if (m_pSniping) m_pSniping->Render(pd3dCommandList);
	m_pMyBOBox->Render(pd3dCommandList); 
#endif // _SHOW_BOUNDINGBOX

	if (!m_isRendering) return; //만약 스나이핑 모드라면 플레이어를 렌더링하지 않는다.
	m_pHaep->UpdateShaderVariable(pd3dCommandList);
	m_pTexture_Cloth->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject_Cloth->Render(pd3dCommandList);
	m_pTexture_Body->UpdateShaderVariable(pd3dCommandList, 0); 
	m_pLoadObject_Body->Render(pd3dCommandList);
}

void Player::RenderHpStatus(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_pPlayerStatus->Render(pd3dCommandList);
}
 
void Player::SetTrackAnimationSet()
{ 
	if (m_CurrAnimation != m_PrevAnimation)
	{
		m_pLoadObject_Cloth->SetTrackAnimationSet(0, m_CurrAnimation);
		m_pLoadObject_Body->SetTrackAnimationSet(0, m_CurrAnimation);
		m_PrevAnimation = m_CurrAnimation;
	}
}

void Player::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
}
 
void Player::Rotate(float x, float y, float z) 
{ 
	m_Transform.Rotate(x, y, z); 
	m_pMyBOBox->Rotate(m_pPlayerMovement->m_fRoll, m_pPlayerMovement->m_fYaw, m_pPlayerMovement->m_fPitch);
}

void Player::ProcessInput(float fTimeElapsed)
{ 
	if (m_isAttacking)
	{
		if (m_pLoadObject_Cloth->IsTrackAnimationSetFinish(0, ANIMATION_ATTACK.ID))
		{
			m_isAttacking = false;
		} 
		return;
	}

	if (m_pBroom->GetisPrepare())
	{
		m_CurrAnimation = ANIMATION_BROOMPREPARE.ID;

		if (m_pLoadObject_Cloth->IsTrackAnimationSetFinish(0, ANIMATION_BROOMPREPARE.ID))
		{
			m_pBroom->DoUse();
		}
		return;
	}

	DWORD dwDirection = 0;
	m_CurrAnimation = ANIMATION_IDLE.ID;
	// m_CurrAnimation = ANIMATION_FORWARD.ID;
	 
	bool isMove = false;
	if (GameInput::IsKeydownW())
	{
		isMove = true;
		m_CurrAnimation = ANIMATION_FORWARD.ID;
		dwDirection |= DIR_FORWARD;
	}
	if (GameInput::IsKeydownS())
	{
		isMove = true;
		m_CurrAnimation = ANIMATION_BACKWARD.ID;
		dwDirection |= DIR_BACKWARD;
	}
	if (GameInput::IsKeydownA())
	{
		isMove = true;
		m_CurrAnimation = ANIMATION_LEFT.ID;
		dwDirection |= DIR_LEFT;
	}
	if (GameInput::IsKeydownD())
	{
		isMove = true;
		m_CurrAnimation = ANIMATION_RIGHT.ID;
		dwDirection |= DIR_RIGHT;
	}

	if (isMove && m_pBroom->GetisUsing())
	{
		m_CurrAnimation = ANIMATION_BROOMFORWARD.ID;
	}
	else if (!isMove && m_pBroom->GetisUsing())
	{
		m_CurrAnimation = ANIMATION_BROOMIDLE.ID;
	}
 
	// 만약 키보드 상하좌우 움직인다면...
	if (dwDirection != 0)
	{  
		//플레이어의 이동량 벡터를 xmf3Shift 벡터만큼 더한다. 
		m_pPlayerMovement->MoveVelocity(dwDirection, fTimeElapsed);
	}
	else
	{
		m_pPlayerMovement->ReduceVelocity(fTimeElapsed);
	}

}

bool Player::Attack()
{
	if (m_pBroom->GetisUsing())
	{
		return false;
	}
	else
	{
		m_pPlayerMovement->m_xmf3Velocity = XMFLOAT3(0.F, 0.F, 0.F);
		m_isAttacking = true;
		m_CurrAnimation = ANIMATION_ATTACK.ID;
		return true;
	}
}

void Player::UseSkill_Broom()
{
	if (!m_pBroom->GetisUsing() && !m_pBroom->GetisPrepare())
	{
		m_pBroom->Prepare();
	}
}

XMFLOAT3 Player::GetVelocity() const
{
	return m_pPlayerMovement->m_xmf3Velocity;
}

void Player::SetVelocity(const XMFLOAT3 & velocity)
{
	m_pPlayerMovement->m_xmf3Velocity = velocity;
}
