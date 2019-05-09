#include "stdafx.h"
#include "PlayerMovement.h"
#include "Broom.h"

void Broom::ReleaseMembers()
{
}

void Broom::ReleaseMemberUploadBuffers()
{
}

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
 	m_UsingTime = 5.f; // 사용 시간
}

Broom::~Broom()
{
}

void Broom::DoUse()
{
	m_isUsing = true;
	m_pPlayerMovement->BroomMode();
}
