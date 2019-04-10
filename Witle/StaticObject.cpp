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

	XMFLOAT3 center{ -30.f, 100.f, 0.f };
	XMFLOAT3 extents{ 100.f, 100.f, 100.f };
	center = Vector3::Add(center, position);
	m_MyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
  
}
 
void MyReflexTree::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int nInstance)
{

}

void MyReflexTree::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_MyBOBox->Render(pd3dCommandList, Matrix4x4::Identity());
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
	m_MyBOBox->Render(pd3dCommandList, Matrix4x4::Identity());
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
	m_MyBOBox->Render(pd3dCommandList, Matrix4x4::Identity());
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
	m_MyBOBox->Render(pd3dCommandList, Matrix4x4::Identity());
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
	m_MyBOBox->Render(pd3dCommandList, Matrix4x4::Identity());
#endif // _SHOW_BOUNDINGBOX

	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("StandardShader")->GetPSO());
	m_LoadObject->Render(pd3dCommandList);
}
//// Altar /////////////////////////////////////////////////////////////
