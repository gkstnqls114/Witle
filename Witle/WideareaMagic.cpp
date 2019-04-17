#include "stdafx.h"
#include "GameObject.h"
#include "CylinderMesh.h"
#include "MyBSphere.h"
#include "WideareaMagic.h"


WideareaMagic::WideareaMagic(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_GameObject = new GameObject("Magic");
	m_CylinderMesh = new CylinderMesh(m_GameObject, pd3dDevice, pd3dCommandList, 100, 100, 300, 10, 10);
	m_CylinderMesh->CreateTexture(pd3dDevice, pd3dCommandList, L"Image/CharacterAppearance1_OFF.dds");
	m_GameObject->InsertComponent("Mesh", m_CylinderMesh);

	m_MyBSphere = new MyBSphere(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0, 0, 0 }, 100);
}

void WideareaMagic::SetPosition(XMFLOAT3 pos)
{
	m_GameObject->GetTransform().SetPosition(pos);
}

void WideareaMagic::Update(float fTimeElapsed)
{
	if (!m_isUsing) return;
	//if (m_CooldownTime < m_SkillTime)
	//{
	//	DoNotUse();
	//}

	//m_SkillTime += fTimeElapsed;
	//std::cout << m_SkillTime << std::endl;
	m_GameObject->GetTransform().Rotate(0.f, rotateValue * fTimeElapsed, 0.f);
	m_GameObject->GetTransform().Update(fTimeElapsed);
}

WideareaMagic::~WideareaMagic()
{
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
	m_MyBSphere->Render(commandList, m_GameObject->GetTransform().GetpWorldMatrixs());
#endif
	UpdateShaderVariable(commandList, m_GameObject->GetTransform().GetpWorldMatrix());	
	m_CylinderMesh->Render(commandList);
}
