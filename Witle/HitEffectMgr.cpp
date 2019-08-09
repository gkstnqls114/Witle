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
		m_MonsterHitEffectList[i].pEffect = new MonsterAttackHitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	} 

	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_IceBallHitEffectList[i].pEffect = new IceBallHitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	}

	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_FireBallHitEffectList[i].pEffect = new FireBallHitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	}

	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_LightningBallHitEffectList[i].pEffect = new LightningBallHitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	}

	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_NormalHitEffectList[i].pEffect = new NormalHitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	}
}

void HitEffectMgr::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_MonsterHitEffectList[i].pEffect->ReleaseUploadBuffers();
		m_IceBallHitEffectList[i].pEffect->ReleaseUploadBuffers();
		m_FireBallHitEffectList[i].pEffect->ReleaseUploadBuffers();
		m_LightningBallHitEffectList[i].pEffect->ReleaseUploadBuffers();
		m_NormalHitEffectList[i].pEffect->ReleaseUploadBuffers();
	} 
}

void HitEffectMgr::ReleaseObjects()
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_MonsterHitEffectList[i].pEffect->ReleaseObjects();
		delete m_MonsterHitEffectList[i].pEffect;
		m_MonsterHitEffectList[i].pEffect = nullptr;
		 
		m_IceBallHitEffectList[i].pEffect->ReleaseObjects();
		delete m_IceBallHitEffectList[i].pEffect;
		m_IceBallHitEffectList[i].pEffect = nullptr;

		m_FireBallHitEffectList[i].pEffect->ReleaseObjects();
		delete m_FireBallHitEffectList[i].pEffect;
		m_FireBallHitEffectList[i].pEffect = nullptr;

		m_LightningBallHitEffectList[i].pEffect->ReleaseObjects();
		delete m_LightningBallHitEffectList[i].pEffect;
		m_LightningBallHitEffectList[i].pEffect = nullptr;

		m_NormalHitEffectList[i].pEffect->ReleaseObjects();
		delete m_NormalHitEffectList[i].pEffect;
		m_NormalHitEffectList[i].pEffect = nullptr; 
	}
}

void HitEffectMgr::Update(float fElapsedTime)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_MonsterHitEffectList[i].pEffect->Update(fElapsedTime);
		m_IceBallHitEffectList[i].pEffect->Update(fElapsedTime);
		m_FireBallHitEffectList[i].pEffect->Update(fElapsedTime);
		m_LightningBallHitEffectList[i].pEffect->Update(fElapsedTime);
		m_NormalHitEffectList[i].pEffect->Update(fElapsedTime);
	}
}

void HitEffectMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_MonsterHitEffectList[i].pEffect->Render(pd3dCommandList, m_MonsterHitEffectList[i].pos);
		m_IceBallHitEffectList[i].pEffect->Render(pd3dCommandList, m_IceBallHitEffectList[i].pos);
		m_FireBallHitEffectList[i].pEffect->Render(pd3dCommandList, m_FireBallHitEffectList[i].pos);
		m_LightningBallHitEffectList[i].pEffect->Render(pd3dCommandList, m_LightningBallHitEffectList[i].pos);
		m_NormalHitEffectList[i].pEffect->Render(pd3dCommandList, m_NormalHitEffectList[i].pos);
	}
}

void HitEffectMgr::AddMonsterHitEffectPosition(const XMFLOAT3 pos)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		if (m_MonsterHitEffectList[i].pEffect->IsActive()) continue;

		m_MonsterHitEffectList[i].pos = pos;
		m_MonsterHitEffectList[i].pEffect->Active();
		break;
	}
}

void HitEffectMgr::AddPlayerSkillHitEffectPosition(ENUM_SELECTABLESKILL skilltype, const XMFLOAT3 pos)
{
	switch (skilltype)
	{
	case SELECTABLESKILL_NONE:
		break;
	case SELECTABLESKILL_FIREBALL:
		AddFireBallHitEffectPosition(pos);
		break;
	case SELECTABLESKILL_ICEBALL:
		AddIceBallHitEffectPosition(pos);
		break;
	case SELECTABLESKILL_LIGHTNINGBALL:
		AddLightningBallHitEffectPosition(pos);
		break;
	case SELECTABLESKILL_SHIELD:
		break;
	case SELECTABLESKILL_BLESSING:
		break;
	case SELECTABLESKILL_HEALING:
		break;
	default:
		break;
	}
}

void HitEffectMgr::AddIceBallHitEffectPosition(const XMFLOAT3 pos)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		if (m_IceBallHitEffectList[i].pEffect->IsActive()) continue;

		m_IceBallHitEffectList[i].pos = pos;
		m_IceBallHitEffectList[i].pEffect->Active();
		break;
	}
}

void HitEffectMgr::AddFireBallHitEffectPosition(const XMFLOAT3 pos)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		if (m_FireBallHitEffectList[i].pEffect->IsActive()) continue;

		m_FireBallHitEffectList[i].pos = pos;
		m_FireBallHitEffectList[i].pEffect->Active();
		break;
	}
}

void HitEffectMgr::AddLightningBallHitEffectPosition(const XMFLOAT3 pos)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		if (m_LightningBallHitEffectList[i].pEffect->IsActive()) continue;

		m_LightningBallHitEffectList[i].pos = pos;
		m_LightningBallHitEffectList[i].pEffect->Active();
		break;
	}
}

void HitEffectMgr::AddNormalHitEffectPosition(const XMFLOAT3 pos)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		if (m_NormalHitEffectList[i].pEffect->IsActive()) continue;

		m_NormalHitEffectList[i].pos = pos;
		m_NormalHitEffectList[i].pEffect->Active();
		break;
	}
}
 