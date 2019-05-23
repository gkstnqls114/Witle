#include "stdafx.h"
#include "MonsterMovement.h"
#include "MyBOBox.h" 
#include "GameInput.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "Sniping.h"
#include "EffectRect.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "Shader.h"
#include "Object.h"
#include "ShaderManager.h" 
#include "Transform.h"
#include "MonsterStatus.h"
#include "GameTimer.h"
#include "FollowCam.h"
#include "Broom.h"
#include "Terrain.h"
#include "Monster.h"

static float MonsterOffestX = 0.f;
static float MonsterOffestY = 57.f;
static float MonsterOffestZ = 50.f;
 
GameObject*  Monster::m_pTarget{ nullptr };

XMFLOAT3 Monster::CalculateAlreadyVelocity(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_MonsterMovement->m_xmf3Velocity, m_MonsterMovement->m_xmf3Gravity);
	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
	float fMaxVelocityXZ = m_MonsterMovement->m_fMaxVelocityXZ;
	if (fLength > m_MonsterMovement->m_fMaxVelocityXZ)
	{
		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_MonsterMovement->m_fMaxVelocityY;
	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
	if (fLength > m_MonsterMovement->m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);

	return xmf3Velocity;
}
BoundingOrientedBox Monster::CalculateAlreadyBoundingBox(float fTimeElapsed)
{
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_MonsterMovement->AlreadyUpdate(fTimeElapsed));
	BoundingOrientedBox AlreadyBBox = m_pMyBOBox->GetBOBox();
	AlreadyBBox.Center = AlreadyPosition;
	return AlreadyBBox;
}
XMFLOAT3 Monster::CalculateAlreadyPosition(float fTimeElapsed)
{
	XMFLOAT3 AlreadyPosition = Vector3::Add(m_Transform.GetPosition(), m_MonsterMovement->AlreadyUpdate(fTimeElapsed));
	return AlreadyPosition;
}

//void Monster::OnCameraUpdateCallback(float fTimeElapsed, Camera* pCamera)
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


Monster::Monster(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
	: GameObject(entityID)
{
	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);

	m_MonsterStatus = new MonsterStatus(this, pd3dDevice, pd3dCommandList);
	m_MonsterMovement = new MonsterMovement(this);
	 
	// m_Transform.SetPosition(100.f, MonsterOffestY, 100.f);// ĳ���Ͱ� �߾ӿ� �����ʾƼ� ��¿������ ����;
	 
}

Monster::~Monster()
{

}

void Monster::ReleaseMembers()
{ 
	if (m_pTexture)
	{
		m_pTexture->ReleaseObjects();
		delete m_pTexture;
		m_pTexture = nullptr;
	} 
	if (m_pLoadObject)
	{
		m_pLoadObject->ReleaseObjects();
		m_pLoadObject = nullptr;
	}
	 
	if (m_MonsterModel)
	{
		m_MonsterModel->ReleaseObjects();
		delete m_MonsterModel;
		m_MonsterModel = nullptr;
	} 
	if (m_pMyBOBox)
	{
		m_pMyBOBox->ReleaseObjects();
		delete m_pMyBOBox;
		m_pMyBOBox = nullptr;
	}
	if (m_MonsterStatus)
	{
		m_MonsterStatus->ReleaseObjects();
		delete m_MonsterStatus;
		m_MonsterStatus = nullptr;
	}
	if (m_MonsterMovement)
	{
		m_MonsterMovement->ReleaseObjects();
		delete m_MonsterMovement;
		m_MonsterMovement = nullptr;
	}
}

void Monster::ReleaseMemberUploadBuffers()
{
	if (m_pTexture) m_pTexture->ReleaseUploadBuffers();
	if (m_pLoadObject) m_pLoadObject->ReleaseUploadBuffers();
	if (m_MonsterModel)m_MonsterModel->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}
 

void Monster::SubstractHP(int sub)
{
	m_CurrAnimation = ANIMATION_BEATTACKED.ID;
	m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation);
	
	m_MonsterStatus->m_HP -= sub;
	std::cout << m_MonsterStatus->m_HP << std::endl;
}

void Monster::Animate(float fElapsedTime)
{
	// animate ������ ���� ������ �ִϸ��̼� �����ϵ��� ����
	SetTrackAnimationSet();

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

void Monster::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX 
	m_pMyBOBox->Render(pd3dCommandList);
#endif // _SHOW_BOUNDINGBOX

	if (!m_isRendering) return; //���� �������� ����� �÷��̾ ���������� �ʴ´�.
	m_pHaep->UpdateShaderVariable(pd3dCommandList);
	m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject->Render(pd3dCommandList);
}

void Monster::RenderHpStatus(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_MonsterStatus->Render(pd3dCommandList);
}

void Monster::SetTrackAnimationSet()
{
	if (m_CurrAnimation != m_PrevAnimation)
	{
		m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation);
		m_PrevAnimation = m_CurrAnimation;
	}
}

void Monster::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
}

void Monster::Rotate(float x, float y, float z)
{
	m_Transform.Rotate(x, y, z);
	m_pMyBOBox->Rotate(m_MonsterMovement->m_fRoll, m_MonsterMovement->m_fYaw, m_MonsterMovement->m_fPitch);
}

void Monster::ProcessInput(float fTimeElapsed)
{
	if (m_CurrAnimation == ANIMATION_BEATTACKED.ID)
	{
		if (!m_pLoadObject->IsTrackAnimationSetFinish(0, ANIMATION_BEATTACKED.ID))
		{
			return;
		}
	}

	//if (isDead)
	//{
	//	m_MonsterStatus->m_HP += 10.f; 
	//	if (m_MonsterStatus->m_HP > 1000.F)
	//	{ 
	//		m_CurrAnimation = ANIMATION_IDLE.ID;
	//		m_Broom->DoNotUse();
	//		m_MonsterStatus->m_HP = 1000.F;
	//		isDead = false;
	//	}
	//	return;
	//}

	//if (m_MonsterStatus->m_HP <= 0 && isDead == false)
	//{
	//	isDead = true;
	//	m_CurrAnimation = ANIMATION_DEAD.ID;
	//	return;
	//}

	if (m_isAttacking)
	{
		if (m_pLoadObject->IsTrackAnimationSetFinish(0, ANIMATION_ATTACK.ID))
		{
			m_isAttacking = false;
		}
		return;
	} 

	DWORD dwDirection = 0;
	m_CurrAnimation = ANIMATION_IDLE.ID;
	// m_CurrAnimation = ANIMATION_DEAD.ID;

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
	 
	// ���� Ű���� �����¿� �����δٸ�...
	if (dwDirection != 0)
	{
		//�÷��̾��� �̵��� ���͸� xmf3Shift ���͸�ŭ ���Ѵ�. 
		m_MonsterMovement->MoveVelocity(dwDirection, fTimeElapsed);
	}
	else
	{
		m_MonsterMovement->ReduceVelocity(fTimeElapsed);
	}

}

void Monster::ProcessInputAI(float fTimeElapsed)
{
	m_CurrAnimation = ANIMATION_IDLE.ID;

}
 

XMFLOAT3 Monster::GetVelocity() const
{
	return m_MonsterMovement->m_xmf3Velocity;
}

void Monster::SetVelocity(const XMFLOAT3 & velocity)
{
	m_MonsterMovement->m_xmf3Velocity = velocity;
}
