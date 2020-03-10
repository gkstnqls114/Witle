#include "stdafx.h"
#include "HealingEffect.h"
#include "PlayerManager.h"
#include "PlayerStatus.h"
#include "Player.h"
#include "HealingSpriteEffect.h"
#include "Healing.h"


void Healing::ReleaseMembers()
{
	SelectableSkill::ReleaseMembers();

	if (m_HealingEffect)
	{
		delete m_HealingEffect;
		m_HealingEffect = nullptr;
	}
}

void Healing::ReleaseMemberUploadBuffers()
{
	SelectableSkill::ReleaseMemberUploadBuffers();

	if (m_HealingEffect) m_HealingEffect->ReleaseUploadBuffers();
}

void Healing::Update(float fElapsedTime)
{ 
	SelectableSkill::Update(fElapsedTime);
	m_HealingEffect->Update(fElapsedTime);
}

void Healing::Render(ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers)
{
	SelectableSkill::Render(pd3dCommandList, isGBuffers);
	
	if (m_HealingEffect)
	{
		m_HealingEffect->Render(pd3dCommandList, m_EffectPos);
	}
}

void Healing::Deactive()
{
}

void Healing::Active()
{
	PlayerManager::GetMainPlayer()->GetHPStatus()->AddHP(10.f);

	if (m_HealingEffect)
	{
		XMFLOAT3 effectPos = PlayerManager::GetMainPlayer()->GetTransform().GetPosition();
		effectPos.y += 75.f;

		m_EffectPos = effectPos;
		m_HealingEffect->Active();
	}
}

bool Healing::IsFinish()
{
	return RemainCoolTime <= 0.f;
}

void Healing::UpdateActive(float fElapsedTime)
{
}

Healing::Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SelectableSkill("Healing", ENUM_SELECTABLESKILL::SELECTABLESKILL_HEALING, COOLTIME_HEALING, 15.f)
{
	m_skillEffect = new HealingEffect("HealingEffect", pd3dDevice, pd3dCommandList);
	m_HealingEffect = new HealingSpriteEffect(this, pd3dDevice, pd3dCommandList);
}

Healing::~Healing()
{
}
