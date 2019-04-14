#include "stdafx.h"
#include "PlayerMovement.h"
#include "Broom.h"

void Broom::DoNotUse()
{
	m_SkillTime = 0.f;
	m_isUsing = false;
	m_pPlayerMovement->RunMode();
}

Broom::Broom(PlayerMovement* playerStatus)
	: Skill()
{
	m_pPlayerMovement = playerStatus;
 	m_CooldownTime = 10.f; // 재사용 대기시간
}

Broom::~Broom()
{
}

void Broom::DoUse()
{
	m_isUsing = true;
	m_pPlayerMovement->BroomMode();
}
