#include "stdafx.h"
#include "HealingEffect.h"
#include "PlayerManager.h"
#include "PlayerStatus.h"
#include "Player.h"
#include "Healing.h"


void Healing::Active()
{
	PlayerManager::GetMainPlayer()->GetHPStatus()->AddHP(10.f);
}

Healing::Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_skillEffect = new HealingEffect("HealingEffect", pd3dDevice, pd3dCommandList);
}


Healing::~Healing()
{
}
