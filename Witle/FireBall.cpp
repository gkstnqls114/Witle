#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"

#include "FireBall.h"
 
void FireBall::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_FireBallMesh->Render(pd3dCommandList, isGBuffers);
}

void FireBall::ReleaseMembers()
{
	m_FireBallMesh->ReleaseObjects();
	delete m_FireBallMesh;
	m_FireBallMesh = nullptr;
}

void FireBall::ReleaseMemberUploadBuffers()
{
	if(m_FireBallMesh) m_FireBallMesh->ReleaseUploadBuffers();
}

FireBall::FireBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SkillEffect(entityID)
{
	m_FireBallMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList);
}

FireBall::~FireBall()
{
}
 