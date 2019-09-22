#include "stdafx.h"

// GameBase //////////////
#include "GameInput.h"
#include "GameTimer.h" 
#include "GameScreen.h"
// GameBase //////////////

#include "SceneMgr.h"

#include "SoundManager.h"
 
#include "Camera.h"
#include "Collision.h"
#include "PlayerMovement.h"
#include "MyBOBox.h" 
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "Sniping.h"
#include "EffectRect.h"
#include "MyRectangle.h"
#include "Shader.h"
#include "Object.h"
#include "ShaderManager.h" 
#include "Transform.h"
#include "PlayerStatus.h"
#include "FollowCam.h"
#include "Broom.h"
#include"GameObject.h"
#include "Terrain.h"
#include "PlayerActionMgr.h"

#include "Dragon.h"

#include "Player.h"

static bool isHitAni = false;

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
		m_PlayerMovement->m_xmf3Velocity = xmf3PlayerVelocity;
		xmf3PlayerPosition.y = fHeight;
		m_Transform.SetPosition(xmf3PlayerPosition);
	}
}
XMFLOAT3 Player::CalculateAlreadyVelocity(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_PlayerMovement->m_xmf3Velocity, m_PlayerMovement->m_xmf3Gravity);
	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
	float fMaxVelocityXZ = m_PlayerMovement->m_fMaxVelocityXZ;
	if (fLength > m_PlayerMovement->m_fMaxVelocityXZ)
	{
		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_PlayerMovement->m_fMaxVelocityY;
	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
	if (fLength > m_PlayerMovement->m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);

	return xmf3Velocity;
}
BoundingOrientedBox Player::CalculateAlreadyBoundingBox(float fTimeElapsed)
{
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_PlayerMovement->AlreadyUpdate(fTimeElapsed));
	BoundingOrientedBox AlreadyBBox = m_pMyBOBox->GetBOBox();
	AlreadyBBox.Center = AlreadyPosition;
	return AlreadyBBox;
}
XMFLOAT3 Player::CalculateAlreadyPosition(float fTimeElapsed)
{
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_PlayerMovement->AlreadyUpdate(fTimeElapsed));
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
	m_pTexture_Cloth = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	m_pTexture_Cloth->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Character_cloth_D.dds", 0);
	m_pTexture_Body = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	m_pTexture_Body->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Character_body_D.dds", 0);
	 
	m_PlayerModel_Cloth = LoadObject::LoadGeometryAndAnimationFromFile_forPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Character_cloth.bin", NULL);
	m_PlayerModel_Body = LoadObject::LoadGeometryAndAnimationFromFile_forPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Character_body.bin", NULL);
	m_pLoadObject_Cloth = m_PlayerModel_Cloth->m_pModelRootObject;
	m_pLoadObject_Body = m_PlayerModel_Body->m_pModelRootObject;
	 
	m_pLoadObject_Cloth->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_PlayerModel_Cloth);
	m_pLoadObject_Cloth->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	m_pLoadObject_Body->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_PlayerModel_Body);
	m_pLoadObject_Body->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	 
	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);
	
	m_pPlayerHPStatus = new PlayerStatus(this, pd3dDevice, pd3dCommandList, 
		POINT{ int(GameScreen::GetWidth()) - 1100, int(GameScreen::GetHeight()) - 670 }, 300.f, 30.f, L"Image/Red.dds");

	m_pPlayerMPStatus = new PlayerStatus(this, pd3dDevice, pd3dCommandList, 
		POINT{ int(GameScreen::GetWidth()) - 1100, int(GameScreen::GetHeight()) - 620 }, 300.f, 30.f, L"Image/Blue.dds");

	m_PlayerMovement = new PlayerMovement(this);
	 
	m_Broom = new Broom(m_PlayerMovement);

	m_Transform.SetPosition(15000.f, 0.f, 15000.f);// 맵 중앙에 위치
	// m_Transform.SetPosition(0.f, 0.f, 0.f);// 맵 왼쪽 하단에 위치

	// 빗자루 스킬 이펙트 준비
	m_BroomEffectRect = new BroomEffectRect(this, pd3dDevice, pd3dCommandList);
	m_BroomLineEffectRect = new MyRectangle(
		this, pd3dDevice, pd3dCommandList, 
		POINT{ int(GameScreen::GetWidth())/2, int(GameScreen::GetHeight()) / 2 }, GameScreen::GetWidth(), GameScreen::GetHeight()
		, L"Image/EffectLine.dds");

	SetUpdatedContext(pContext); 

	m_PlayerActionMgr = new PlayerActionMgr(this);

	Init();
}

Player::~Player()
{ 
	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;
}

void Player::Init()
{
	m_PlayerActionMgr->Init();

	m_Transform.SetIdentity();
	m_Transform.SetPosition(15000, 0, 15000);
	  
	isDead = false;
	m_isAttacking = false;
	m_PlayerMovement->RunMode();
	m_pPlayerMPStatus->SetGuage(100.f);
	m_pPlayerHPStatus->SetGuage(100.f);
}

void Player::ActiveShield()
{
	shield = 5.f;
}

void Player::DeactiveShield()
{
	shield = 0.f;
}

void Player::ActiveBlessing()
{
	blessing = 1.7f;
	if (m_Broom->GetisUsing())
	{
		m_PlayerMovement->BroomMode();
	}
	else
	{ 
		m_PlayerMovement->RunMode();
	}
}

void Player::DeactiveBlessing()
{
	blessing = 1.f;
	if (m_Broom->GetisUsing())
	{
		m_PlayerMovement->BroomMode();
	}
	else
	{
		m_PlayerMovement->RunMode();
	}
}
 
void Player::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	if (m_pSniping) m_pSniping->Render(pd3dCommandList, isGBuffers);
	
	if (!m_isRendering) return; //만약 스나이핑 모드라면 플레이어를 렌더링하지 않는다.

	m_pTexture_Cloth->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject_Cloth->Render(pd3dCommandList, isGBuffers);
	m_pTexture_Body->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject_Body->Render(pd3dCommandList, isGBuffers);

	bool isMoving = GameInput::IsKeydownW() || GameInput::IsKeydownA() || GameInput::IsKeydownS() || GameInput::IsKeydownD();
	if (m_Broom->GetisUsing() && isMoving)
	{
		m_BroomLineEffectRect->Render(pd3dCommandList, XMFLOAT2(0.f, 0.f), CGameTimer::GetInstance()->GetTotalTime());
	}
}

void Player::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{  
	m_pLoadObject_Cloth->RenderForPlayerShadow(pd3dCommandList, this);
	
	m_pLoadObject_Body->RenderForPlayerShadow(pd3dCommandList, this);
}

void Player::ReleaseMembers()
{
	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;
	 
	if (m_PlayerActionMgr)
	{
		m_PlayerActionMgr->ReleaseObjects();
		delete m_PlayerActionMgr;
		m_PlayerActionMgr = nullptr;
	}
	if (m_BroomEffectRect)
	{
		m_BroomEffectRect->ReleaseObjects();
		delete m_BroomEffectRect;
		m_BroomEffectRect = nullptr;
	}
	if (m_BroomLineEffectRect)
	{
		m_BroomLineEffectRect->ReleaseObjects();
		delete m_BroomLineEffectRect;
		m_BroomLineEffectRect = nullptr;
	}
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
	if (m_Broom)
	{ 
		m_Broom->ReleaseObjects();
		delete m_Broom;
		m_Broom = nullptr;
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
	if (m_pPlayerHPStatus)
	{
		m_pPlayerHPStatus->ReleaseObjects();
		delete m_pPlayerHPStatus;
		m_pPlayerHPStatus = nullptr;
	}

	if (m_pPlayerMPStatus)
	{
		m_pPlayerMPStatus->ReleaseObjects();
		delete m_pPlayerMPStatus;
		m_pPlayerMPStatus = nullptr;
	}
	if (m_PlayerMovement)
	{
		m_PlayerMovement->ReleaseObjects();
		delete m_PlayerMovement;
		m_PlayerMovement = nullptr;
	}
}

void Player::ReleaseMemberUploadBuffers()
{
	if (m_BroomEffectRect) m_BroomEffectRect->ReleaseUploadBuffers();
	if (m_BroomLineEffectRect) m_BroomLineEffectRect->ReleaseUploadBuffers();
	if (m_pTexture_Cloth) m_pTexture_Cloth->ReleaseUploadBuffers(); 
	if (m_pTexture_Body) m_pTexture_Body->ReleaseUploadBuffers(); 
	if (m_pLoadObject_Cloth) m_pLoadObject_Cloth->ReleaseUploadBuffers();
	if (m_pLoadObject_Body) m_pLoadObject_Body->ReleaseUploadBuffers();
	if (m_PlayerModel_Cloth)m_PlayerModel_Cloth->ReleaseUploadBuffers();
	if (m_PlayerModel_Body)m_PlayerModel_Body->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}
 
void Player::SetAnimationID(ENUM_ANIMATIONID state)
{  
	m_PlayerActionMgr->SetUpPrevActionID();
	m_pLoadObject_Cloth->SetTrackAnimationSet(0, state);
	m_pLoadObject_Body->SetTrackAnimationSet(0, state);
}

void Player::Update(float fElapsedTime)
{  
	SoundManager::GetInstance()->UpdateListenerPos(this);

	// 초당 hp, mp회복
	if (!isDead)
	{ 
	m_pPlayerHPStatus->AddHP(5 * fElapsedTime);
	m_pPlayerMPStatus->AddHP(10 * fElapsedTime);
	}

	m_Broom->Update(fElapsedTime);

	// 이동량을 계산한다. 
	m_PlayerMovement->Update(fElapsedTime);
	
	// 이동량만큼 움직인다. 
	Move(Vector3::ScalarProduct(m_PlayerMovement->m_xmf3Velocity, fElapsedTime, false));

	m_pMyBOBox->SetPosition(
		XMFLOAT3(
		m_Transform.GetPosition().x, 
		75.f,
		m_Transform.GetPosition().z)
	);
	
}
 
static bool isBrooming = false;
void Player::SubstractHP(int sub)
{  
	if (isBrooming) return;
	if (m_PlayerActionMgr->Is_HitAction()) return;
	
	float prevDamage = m_pPlayerHPStatus->GetGuage();
	m_pPlayerHPStatus->SubstractHP(sub);
	
	if (m_PlayerActionMgr->Is_BroomIdleAction())
	{
		isBrooming = true; 
		return;
	}
	if (m_PlayerActionMgr->Is_BroomForwardAction())
	{
		isBrooming = true; 
		return;
	}
	if (m_PlayerActionMgr->Is_BroomPrepareAction())
	{
		isBrooming = true; 
		return;
	}
	 
	if (m_PlayerActionMgr->Is_HitAction())
	{
		m_PlayerActionMgr->ChangeActionToHit();
		m_pLoadObject_Body->SetTrackAnimationSet(0, m_PlayerActionMgr->GetCurrPlayerAction()->m_AnimationID);
		m_pLoadObject_Cloth->SetTrackAnimationSet(0, m_PlayerActionMgr->GetCurrPlayerAction()->m_AnimationID);
	}

}

void Player::Animate(float fElapsedTime)
{ 
	// 반드시 트랜스폼 업데이트..! 
	m_Transform.Update(fElapsedTime);
	 
	m_pLoadObject_Cloth->m_xmf4x4ToParent = m_Transform.GetWorldMatrix();
	m_pLoadObject_Body->m_xmf4x4ToParent = m_Transform.GetWorldMatrix();
	
	m_pLoadObject_Cloth->Animate(fElapsedTime);
	m_pLoadObject_Body->Animate(fElapsedTime);
}

void Player::RenderStatus(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_pPlayerHPStatus->Render(pd3dCommandList);
	m_pPlayerMPStatus->Render(pd3dCommandList);
}
  
void Player::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
}
 
void Player::Rotate(float x, float y, float z) 
{ 
	m_Transform.Rotate(x, y, z); 
	m_pMyBOBox->Rotate(m_PlayerMovement->m_fRoll, m_PlayerMovement->m_fYaw, m_PlayerMovement->m_fPitch);
}
 
static float hittime = 0.f;
void Player::ProcessInput(float fTimeElapsed)
{    
	m_PlayerActionMgr->UpdateState(fTimeElapsed);
	m_PlayerActionMgr->UpdateVelocity(fTimeElapsed, m_PlayerMovement);
	 
	if (isDead) return;

	if (m_pPlayerHPStatus->GetGuage() <= 0 && isDead == false)
	{
		isDead = true;
		m_PlayerActionMgr->ChangeActionToDead();
		return;
	}

	if (m_PlayerActionMgr->Is_HitAction() || isBrooming)
	{  
		hittime += fTimeElapsed;

		if (hittime < (ANIMATION_HIT.EndTime - ANIMATION_HIT.StartTime))
		{
			return;
		}
		else
		{
			m_PlayerActionMgr->ChangeActionToIdle();
			hittime = 0.f;
			m_isAttacking = false;
			isBrooming = false;
			return;
		}
	}

	//if (isDead)
	//{
	//	m_pPlayerHPStatus->m_Guage += 10.f; 
	//	if (m_pPlayerHPStatus->m_Guage > 1000.F)
	//	{ 
	//		m_CurrAnimation = ANIMATION_IDLE.ID;
	//		m_Broom->DoNotUse();
	//		m_pPlayerHPStatus->m_Guage = 1000.F;
	//		isDead = false;
	//	}
	//	return;
	//}

	if (m_isAttacking)
	{
		if (m_pLoadObject_Cloth->IsTrackAnimationSetFinish(0, ANIMATION_ATTACK.ID) && 
			m_pLoadObject_Body->IsTrackAnimationSetFinish(0, ANIMATION_ATTACK.ID))
		{
			m_isAttacking = false;
		} 
		return;
	}

	if (m_Broom->GetisPrepare())
	{
		m_PlayerActionMgr->ChangeActionToBroomPrepare();

		if (m_pLoadObject_Cloth->IsTrackAnimationSetFinish(0, ANIMATION_BROOMPREPARE.ID) &&
			m_pLoadObject_Body->IsTrackAnimationSetFinish(0, ANIMATION_BROOMPREPARE.ID))
		{
			m_Broom->DoUse();
		}
		return;
	}

	DWORD dwDirection = 0;
	m_PlayerActionMgr->ChangeActionToIdle();
	 
	bool isMove = false;
	if (GameInput::IsKeydownW())
	{
		isMove = true;
		m_PlayerActionMgr->ChangeActionToForwardWalk();
		dwDirection |= DIR_FORWARD;
	}
	if (GameInput::IsKeydownS())
	{
		isMove = true;
		m_PlayerActionMgr->ChangeActionToBackwardWalk();
		dwDirection |= DIR_BACKWARD;
	}
	if (GameInput::IsKeydownA())
	{
		isMove = true;
		m_PlayerActionMgr->ChangeActionToLeftWalk();
		dwDirection |= DIR_LEFT;
	}
	if (GameInput::IsKeydownD())
	{
		isMove = true;
		m_PlayerActionMgr->ChangeActionToRightWalk();
		dwDirection |= DIR_RIGHT;
	}

	if (isMove && m_Broom->GetisUsing())
	{ 
		m_PlayerActionMgr->ChangeActionToBroomForward(); 
	}
	else if (!isMove && m_Broom->GetisUsing())
	{
		m_PlayerActionMgr->ChangeActionToBroomIdle(); 
	}
 
	// 만약 키보드 상하좌우 움직인다면...
	if (dwDirection != 0)
	{  
		//플레이어의 이동량 벡터를 xmf3Shift 벡터만큼 더한다. 
		m_PlayerMovement->MoveVelocity(dwDirection, fTimeElapsed);
	}
	else
	{
		m_PlayerMovement->ReduceVelocity(fTimeElapsed);
	}

}

void Player::ProcessInputAI(float fTimeElapsed)
{  

}

//  
bool Player::Attack(Status* status, MyCollider* collider, XMFLOAT2 aimPoint, Camera* pMainCaemra, bool isDragon = false)
{   
	if (isDead) return false;

	// 시행된다면..
	bool isAttack = false;

	bool isNearMonster = Vector3::Length(collider->GetpOwner()->GetTransform().GetPosition(), m_Transform.GetPosition()) < 100;
	if (isNearMonster) // 몬스터와 가까운 경우 근접 공격
	{   
		isAttack = true;
		if (isDragon)
		{  
			status->SubstractHP(10);
		}
		else
		{
			status->Damage(30, ANIMATION_HIT.ID);
		}
	}
	else
	{  
		// 피킹 ray를 만든다.
		RAY pickRay = RAY::GeneratePickingRay(aimPoint, pMainCaemra);

		// 다른 유저와 닿는지 확인한다.
		float Playerdist;
		bool isCollide = Collision::isCollide(collider, pickRay.origin, pickRay.direction, Playerdist);

		if (isCollide && Playerdist < 3000.f)
		{  
			isAttack = true;
			if (isDragon)
			{
				status->SubstractHP(10);
			}
			else
			{
				status->Damage(30, ANIMATION_HIT.ID);
			}
		}

	}

	m_PlayerMovement->m_xmf3Velocity = XMFLOAT3(0.F, 0.F, 0.F);
	m_isAttacking = true; 
	m_PlayerActionMgr->ChangeActionToStandardAttack();
	return isAttack;
}

void Player::UseSkill_Broom()
{
	if (!m_Broom->GetisUsing() && !m_Broom->GetisPrepare())
	{
		m_Broom->Prepare();
	}
}
 

XMFLOAT3 Player::GetVelocity() const
{
	return m_PlayerMovement->m_xmf3Velocity;
}

bool Player::IsTrackAnimationSetFinish(ENUM_ANIMATIONID id) const
{
	return (m_pLoadObject_Cloth->IsTrackAnimationSetFinish(0, id) && m_pLoadObject_Body->IsTrackAnimationSetFinish(0, id));
}

void Player::SetVelocity(const XMFLOAT3 & velocity)
{
	m_PlayerMovement->m_xmf3Velocity = velocity;
}
