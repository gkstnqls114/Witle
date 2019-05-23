#include "stdafx.h"
#include "d3dUtil.h"
#include "Object.h"
#include "MyBOBox.h"
#include "Shader.h"
#include "Object.h"
#include "ShaderManager.h"
#include "ModelStorage.h"
#include "StaticObject.h"

void StaticObject::ReleaseMembers()
{
}

void StaticObject::ReleaseMemberUploadBuffers()
{
}
  
StaticObject::StaticObject(const std::string &EntityID)
	:GameObject(EntityID)
{
}

//// MyReflexTree /////////////////////////////////////////////////////////////

MyReflexTree::MyReflexTree(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("ReflexTree")
{ 
	ModelStorage::GetInstance()->CreateModels(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	m_LoadObject = ModelStorage::GetInstance()->GetRootObject("ReflexTree");

	m_LoadObject->SetPosition(position);
	 
	XMFLOAT3 extents{ 100.f, 100.f, 100.f };
	m_pMyBOBox = new MyBOBox(this , pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents);
  
}
 
void MyReflexTree::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int nInstance)
{

}

void MyReflexTree::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_pMyBOBox->Render(pd3dCommandList);
#endif // _SHOW_BOUNDINGBOX

	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("InstancingStandardShader")->GetPSO());
	m_LoadObject->Render(pd3dCommandList);
}
//// MyReflexTree /////////////////////////////////////////////////////////////
 

//// SunFlower /////////////////////////////////////////////////////////////
SunFlower::SunFlower(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("Sunflower") 
{
	ModelStorage::GetInstance()->CreateModels(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_LoadObject = ModelStorage::GetInstance()->GetRootObject("Sunflower");
}
 
void SunFlower::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_pMyBOBox->Render(pd3dCommandList);
#endif // _SHOW_BOUNDINGBOX

	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("StandardShader")->GetPSO());
	m_LoadObject->Render(pd3dCommandList);
}
//// SunFlower /////////////////////////////////////////////////////////////



//// Rock /////////////////////////////////////////////////////////////
Rock::Rock(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("Rock") 
{
}
 
void Rock::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_pMyBOBox->Render(pd3dCommandList);
#endif // _SHOW_BOUNDINGBOX

	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("StandardShader")->GetPSO());
	m_LoadObject->Render(pd3dCommandList);
}
//// Rock /////////////////////////////////////////////////////////////



//// Pillar /////////////////////////////////////////////////////////////

Pillar::Pillar(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("Pilla")
{
}

void Pillar::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_pMyBOBox->Render(pd3dCommandList);
#endif // _SHOW_BOUNDINGBOX

	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("StandardShader")->GetPSO());
	m_LoadObject->Render(pd3dCommandList);
}
//// Pillar /////////////////////////////////////////////////////////////
 

//// Altar /////////////////////////////////////////////////////////////

Altar::Altar(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("Altar") 
{
}

void Altar::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_pMyBOBox->Render(pd3dCommandList);
#endif // _SHOW_BOUNDINGBOX

	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("StandardShader")->GetPSO());
	m_LoadObject->Render(pd3dCommandList);
}
//// Altar /////////////////////////////////////////////////////////////
