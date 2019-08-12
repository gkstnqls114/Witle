#include "stdafx.h"
#include "BlessingEffect.h"

#include "PlayerManager.h"
#include "Player.h"

#include "Blessing.h"


void Blessing::Active()
{ 
	PlayerManager::GetMainPlayer()->ActiveBlessing();
}

bool Blessing::IsFinish()
{
	bool isfinish =  (m_UsingTime >= m_MAXUsingTime);
	if (isfinish)
	{
		PlayerManager::GetMainPlayer()->DeactiveBlessing();
		m_UsingTime = 0.f;
	}
	return isfinish;
}

void Blessing::UpdateActive(float fElapsedTime)
{
	if (m_UsingTime >= m_MAXUsingTime)
	{
		m_UsingTime = m_MAXUsingTime;
	}
	else
	{
		m_UsingTime += fElapsedTime;
	}
}

Blessing::Blessing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SelectableSkill("Blessing", ENUM_SELECTABLESKILL::SELECTABLESKILL_BLESSING, COOLTIME_BLESSING, 15.f)
{
	m_skillEffect = new BlessingEffect("BlessingEffect", pd3dDevice, pd3dCommandList);
}


Blessing::~Blessing()
{
}
