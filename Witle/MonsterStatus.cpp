#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "ShaderManager.h"
#include "MonsterStatus.h"

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
	: ComponentBase(pOwner)
{
	m_HpBar = new MyRectangle(pOwner, pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) - 200, int(GameScreen::GetHeight()) - 70 }, 300.f, 30.f, nullptr);
}

MonsterStatus::~MonsterStatus()
{
}

void MonsterStatus::Update(float fTimeElapsed)
{

}

void MonsterStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	float percentage = float(m_HP) / 1000.f * 100.f;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &percentage, 0);
	m_HpBar->Render(pd3dCommandList);
}
