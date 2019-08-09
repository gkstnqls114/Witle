#include "stdafx.h"

#include "GameScreen.h"
#include "UI3DImage.h" 
#include "Monster.h"
#include "MainCameraMgr.h" 
#include "ShaderManager.h"
#include "Monster.h" 
#include "CameraObject.h"
#include "GameObject.h"
#include "MonsterAttackHitEffect.h"
    
 
void MonsterAttackHitEffect::ReleaseObjects()
{
	if (m_MonsterAttackHitEffect)
	{
		m_MonsterAttackHitEffect->ReleaseObjects();
		delete m_MonsterAttackHitEffect;
		m_MonsterAttackHitEffect = nullptr;
	}
}

void MonsterAttackHitEffect::ReleaseUploadBuffers()
{
	if (m_MonsterAttackHitEffect) m_MonsterAttackHitEffect->ReleaseUploadBuffers();
}

MonsterAttackHitEffect::MonsterAttackHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: ComponentBase(pOwner)
{
	m_ResolX = 5;
	m_ResolY = 1;
	m_MonsterAttackHitEffect = new UI3DImage(pOwner, pd3dDevice, pd3dCommandList, POINT{ 0, 0 }, 100, 123,
	L"Image/sprite test2.dds");
}

MonsterAttackHitEffect::~MonsterAttackHitEffect()
{

}

void MonsterAttackHitEffect::Update(float fTimeElapsed)
{
	if (!isActive) return;

	m_TotalTime += fTimeElapsed;

	if (m_TotalTime > m_ChangeIndexTime)
	{
		m_TotalTime -= m_ChangeIndexTime;
		m_CurrIndex += 1;
		if (m_CurrIndex > m_ResolX * m_ResolY - 1)
		{
			isActive = false;
			m_CurrIndex = 0;
		}
	}
}

void MonsterAttackHitEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT3& pos)
{
	if (!isActive) return;

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_HITEFFECT, false);

	// set look at.... ºôº¸µå Ã³¸®...  
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(pos, MainCameraMgr::GetMainCamera()->GetTransform().GetPosition(), XMFLOAT3(0, 1.f, 0));
	XMFLOAT4X4 uiWorld = Matrix4x4::Identity();

	uiWorld._11 = mtxLookAt._11; uiWorld._12 = mtxLookAt._21; uiWorld._13 = mtxLookAt._31;
	uiWorld._21 = mtxLookAt._12; uiWorld._22 = mtxLookAt._22; uiWorld._23 = mtxLookAt._32;
	uiWorld._31 = mtxLookAt._13; uiWorld._32 = mtxLookAt._23; uiWorld._33 = mtxLookAt._33; 
	uiWorld._41 = pos.x;
	uiWorld._42 = pos.y;
	uiWorld._43 = pos.z;
	// set look at....  

	XMFLOAT2 Resol{ m_ResolX, m_ResolY };

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &Resol, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &m_CurrIndex, 0);

	m_MonsterAttackHitEffect->Render(pd3dCommandList, uiWorld);
}
 