#include "stdafx.h"
#include "GameObject.h"
#include "CylinderMesh.h"
#include "WideareaMagic.h"


WideareaMagic::WideareaMagic(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_GameObject = new GameObject("Magic");
	m_CylinderMesh = new CylinderMesh(m_GameObject, pd3dDevice, pd3dCommandList, 10, 10, 100, 10, 10);
	m_GameObject->InsertComponent("Mesh", m_CylinderMesh);
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
	m_CylinderMesh->Render(commandList);
}
