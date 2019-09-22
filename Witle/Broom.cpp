#include "stdafx.h"
#include "PlayerMovement.h"
#include "Broom.h"

void Broom::ReleaseMembers()
{
}

void Broom::ReleaseMemberUploadBuffers()
{
}

void Broom::PrepareMember()
{
	m_PlayerMovement->m_xmf3Velocity = XMFLOAT3(0.F, 0.F, 0.F);
}

void Broom::DoNotUse()
{
	m_SkillTime = 0.f;
	m_isUsing = false;
	m_isPrepare = false;
	m_PlayerMovement->RunMode();
}

Broom::Broom(PlayerMovement* playerStatus)
	: FixedSkill("Broom")
{
	m_PlayerMovement = playerStatus;
 	m_UsingTime = 5.f; // 사용 시간
}

Broom::~Broom()
{
}

void Broom::DoUse()
{
	m_isUsing = true;
	m_isPrepare = false;
	m_PlayerMovement->BroomMode();
}
