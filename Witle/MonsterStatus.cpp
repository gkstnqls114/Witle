#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "Monster.h"
#include "GeneralMonsterActionMgr.h"
#include "ShaderManager.h"
#include "Monster.h"
#include "GeneralMonsterActionMgr.h"
#include "MonsterMovement.h"
#include "GameObject.h"
#include "MonsterStatus.h"

void MonsterStatus::ChangeAfterActionToHit()
{ 
	static_cast<Monster*>(m_pOwner)->SetAnimationID(SPACECAT_HIT.ID);
}

void MonsterStatus::ReleaseObjects()
{
	if (m_HpBar)
	{
		m_HpBar->ReleaseObjects();
		delete m_HpBar;
		m_HpBar = nullptr;
	}
}

void MonsterStatus::ReleaseUploadBuffers()
{
	if (m_HpBar) m_HpBar->ReleaseUploadBuffers();
}

MonsterStatus::MonsterStatus(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: Status(pOwner)
{

}

MonsterStatus::~MonsterStatus()
{
}

void MonsterStatus::Update(float fTimeElapsed)
{

}
 
void MonsterStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{

}

void MonsterStatus::Damage(int damage, int state)
{
	ActionMgr* pMonsterActionMgr = static_cast<Monster*>(m_pOwner)->GetActionMgr();
	GeneralMonsterActionMgr* pGeneralMonsterActionMgr = static_cast<GeneralMonsterActionMgr*>(pMonsterActionMgr);
	if (pGeneralMonsterActionMgr->isDead()) return; // 만약에 죽은 경우 리턴한다.

	pGeneralMonsterActionMgr->ChangeStateToHit();
	SubstractHP(damage);
}
