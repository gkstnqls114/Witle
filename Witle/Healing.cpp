#include "stdafx.h"
#include "HealingEffect.h"
#include "PlayerManager.h"
#include "PlayerStatus.h"
#include "Player.h"
#include "Healing.h"


void Healing::Deactive()
{
}

void Healing::Active()
{
	PlayerManager::GetMainPlayer()->GetHPStatus()->AddHP(10.f);
}

bool Healing::IsFinish()
{
	return true;
}

void Healing::UpdateActive(float fElapsedTime)
{
}

Healing::Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SelectableSkill("Healing", ENUM_SELECTABLESKILL::SELECTABLESKILL_HEALING, COOLTIME_HEALING, 15.f)
{
	m_skillEffect = new HealingEffect("HealingEffect", pd3dDevice, pd3dCommandList);
}

Healing::~Healing()
{
}
