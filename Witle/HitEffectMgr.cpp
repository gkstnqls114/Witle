#include "stdafx.h"

// Hit Effect 클래스들
#include "NormalHitEffect.h"
#include "IceBallHitEffect.h"
#include "FireBallHitEffect.h"
#include "LightningBallHitEffect.h"
#include "MonsterAttackHitEffect.h"
#include "NormalHitEffect.h"
// Hit Effect 클래스들

#include "GameObject.h"

#include "HitEffectMgr.h"

HitEffectMgr* HitEffectMgr::m_Instance{ nullptr };
 
HitEffectMgr::~HitEffectMgr()
{
}

void HitEffectMgr::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_GameObject = new EmptyGameObject("test");

	for (int i = 0 ; i < m_MaxEffect ; ++i)
	{
		m_EffectList[i].pEffect = new NormalHitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	} 
}

void HitEffectMgr::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->ReleaseUploadBuffers(); 
	} 
}

void HitEffectMgr::ReleaseObjects()
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->ReleaseObjects();
		delete m_EffectList[i].pEffect;
		m_EffectList[i].pEffect = nullptr;
	}
}

void HitEffectMgr::Update(float fElapsedTime)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->Update(fElapsedTime);
	}
}

void HitEffectMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->Render(pd3dCommandList, m_EffectList[i].pos);
	}
}

void HitEffectMgr::AddEffectPosition(ENUM_EFFECT type, const XMFLOAT3 pos)
{
	switch (type)
	{
	case EFFECT_NONE:

		break;
	case EFFECT_TEST:
		for (int i = 0; i < m_MaxEffect; ++i)
		{
			if (m_EffectList[i].pEffect->IsActive()) continue;
			 
			m_EffectList[i].pos = pos;
			m_EffectList[i].pEffect->Active();
			break;
		}
		break;
	default:
		break;
	}
} 