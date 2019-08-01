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
	m_pPlayerMovement->m_xmf3Velocity = XMFLOAT3(0.F, 0.F, 0.F);
}

void Broom::DoNotUse()
{
	m_SkillTime = 0.f;
	m_isUsing = false;
	m_isPrepare = false;
	m_pPlayerMovement->RunMode();
}

Broom::Broom(PlayerMovement* playerStatus)
	: FixedSkill()
{
	m_pPlayerMovement = playerStatus;
 	m_UsingTime = 5.f; // ��� �ð�
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
