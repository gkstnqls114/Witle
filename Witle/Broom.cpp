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
	m_pPlayerMovement->SetVelocity( XMFLOAT3(0.F, 0.F, 0.F));
}

void Broom::Init()
{
	FixedSkill::Init();
}

void Broom::DoNotUse()
{
	m_SkillTime = 0.f;
	m_isUsing = false;
	m_isPrepare = false;
	m_pPlayerMovement->RunMode();
}

Broom::Broom(PlayerMovement* playerStatus)
	: FixedSkill("Broom")
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
	m_isPrepare = false;
	m_pPlayerMovement->BroomMode();
}
