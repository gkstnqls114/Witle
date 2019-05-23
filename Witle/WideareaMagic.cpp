#include "stdafx.h"
#include "GameObject.h"
#include "CylinderMesh.h"
#include "MyBSphere.h"
#include "WideareaMagic.h"


WideareaMagic::WideareaMagic(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{ 
	m_CylinderMesh = new CylinderMesh(this, pd3dDevice, pd3dCommandList, 100, 100, 300, 10, 10);
	m_CylinderMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/CharacterAppearance1_OFF.dds");
	 
	m_MyBSphere = new MyBSphere(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0, 0, 0 }, 100);
}

BoundingSphere* WideareaMagic::GetBSphere()
{
	return nullptr;
}

void WideareaMagic::SetPosition(XMFLOAT3 pos)
{
	m_Transform.SetPosition(pos);
}

void WideareaMagic::Update(float fTimeElapsed)
{
	if (!m_isUsing) return;
	//if (m_UsingTime < m_SkillTime)
	//{
	//	DoNotUse();
	//}

	//m_SkillTime += fTimeElapsed;
	//std::cout << m_SkillTime << std::endl;
	m_Transform.Rotate(0.f, rotateValue * fTimeElapsed, 0.f);
	m_Transform.Update(fTimeElapsed);
}

WideareaMagic::~WideareaMagic()
{
}

void WideareaMagic::ReleaseMembers()
{ 
	if (m_MyBSphere)
	{
		m_MyBSphere->ReleaseObjects();
		delete m_MyBSphere;
		m_MyBSphere = nullptr;
	}
	if (m_CylinderMesh)
	{
		m_CylinderMesh->ReleaseObjects();
		delete m_CylinderMesh;
		m_CylinderMesh = nullptr;
	}
}

void WideareaMagic::ReleaseMemberUploadBuffers()
{
	if (m_MyBSphere) m_MyBSphere->ReleaseUploadBuffers(); 
	if (m_CylinderMesh) m_CylinderMesh->ReleaseUploadBuffers(); 
}

void WideareaMagic::DoNotUse()
{
	m_isUsing = false;
}

void WideareaMagic::DoUse()
{
	m_isUsing = true;
	
}

void WideareaMagic::Render(ID3D12GraphicsCommandList* commandList)
{
	if (!m_isUsing) return; 

#ifdef _SHOW_BOUNDINGBOX
	m_MyBSphere->Render(commandList, m_Transform.GetpWorldMatrixs());
#endif
	UpdateShaderVariable(commandList, m_Transform.GetpWorldMatrix());
	m_CylinderMesh->Render(commandList);
}
