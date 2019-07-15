#include "stdafx.h"

#include "GraphicsRootSignatureMgr.h"
#include "SphereMesh.h"
#include "MyBSphere.h"

#include "FireBall.h"
 
void FireBall::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	m_FireBallMesh->Render(pd3dCommandList, isGBuffers);
}

void FireBall::UpdateCollider()
{
	static_cast<MyBSphere*>(m_MyCollider)->GetBSphere()->Center = m_Transform.GetPosition();
}

void FireBall::ReleaseMembers()
{
	SkillEffect::ReleaseMembers();

	m_FireBallMesh->ReleaseObjects();
	delete m_FireBallMesh;
	m_FireBallMesh = nullptr;
}

void FireBall::ReleaseMemberUploadBuffers()
{
	SkillEffect::ReleaseMemberUploadBuffers();

	if(m_FireBallMesh) m_FireBallMesh->ReleaseUploadBuffers();
}

FireBall::FireBall(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SkillEffect(entityID, 5.f)
{
	m_FireBallMesh = new SphereMesh(this, pd3dDevice, pd3dCommandList, 50, 50, 10, 10);
	m_FireBallMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/Red.dds");

	m_MyCollider = new MyBSphere(this, pd3dDevice, pd3dCommandList, 50);
}

FireBall::~FireBall()
{

}
 