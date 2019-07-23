#include "stdafx.h"

// Skill Effect 관련 ////////////////////////////
#include "SkillEffect.h"
#include "FireBall.h"
#include "IceBall.h"
#include "LightningBall.h" 
// Skill Effect 관련 ////////////////////////////

#include "SkillStg.h"

SkillStg* SkillStg::m_Instance{ nullptr };

SkillStg::SkillStg()
{
}

SkillStg::~SkillStg()
{
}

void SkillStg::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	skillEffect[0] = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	skillEffect[1] = new IceBall("IceBall", pd3dDevice, pd3dCommandList);
	skillEffect[2] = new LightningBall("LightingBall", pd3dDevice, pd3dCommandList);

	skillEffect[3] = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	skillEffect[4] = new IceBall("FireBall", pd3dDevice, pd3dCommandList);
	skillEffect[5] = new LightningBall("FireBall", pd3dDevice, pd3dCommandList);

	skillEffect[6] = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	skillEffect[7] = new IceBall("FireBall", pd3dDevice, pd3dCommandList); 
}

void SkillStg::ReleaseUploadBuffers()
{
	for (auto& se : skillEffect)
	{
		se->ReleaseUploadBuffers();
	}
}

void SkillStg::ReleaseObjects()
{
	for (auto& se : skillEffect)
	{
		se->ReleaseObjects();
		delete se;
		se = nullptr;
	}
}
 