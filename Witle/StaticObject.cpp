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
std::vector<XMFLOAT4X4> MyReflexTree::m_PostionFromMap(1000);
int MyReflexTree::m_CountFromMap = 0;
int MyReflexTree::m_Count = 0;
ID3D12Resource* MyReflexTree::m_pd3dcbGameObjects { nullptr };
VS_SRV_INSTANCEINFO* MyReflexTree::m_pcbMappedGameObjects{ nullptr };

void MyReflexTree::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	m_pd3dcbGameObjects = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(VS_SRV_INSTANCEINFO) * m_CountFromMap, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);

	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
}

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

MyReflexTree::MyReflexTree(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: StaticObject("ReflexTree") 
{ 
	ModelStorage::GetInstance()->CreateModels(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	m_LoadObject = ModelStorage::GetInstance()->GetRootObject("ReflexTree");

	XMFLOAT4X4 transform = m_PostionFromMap[m_Count];

	XMFLOAT3 position =
	{
		- transform._41,
		transform._42,
		- transform._43,
	};

	m_LoadObject->SetPosition(position); 
	m_LoadObject->UpdateTransform(NULL);

	m_PostionFromMap[m_Count] = m_LoadObject->m_pChild->m_xmf4x4World;
	m_Count += 1;

	XMFLOAT3 center{ -30.f, 100.f, 0.f };
	XMFLOAT3 extents{ 100.f, 100.f, 100.f };

	center = Vector3::Add(center, position);
	m_MyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
}
 
void MyReflexTree::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int nInstance)
{
	pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, m_pd3dcbGameObjects->GetGPUVirtualAddress());
	 
	for (UINT x = 0; x < nInstance; x++)
	{ 
		XMStoreFloat4x4(&m_pcbMappedGameObjects[x].m_xmf4x4Transform, XMMatrixTranspose(XMLoadFloat4x4(&m_PostionFromMap[x]))); 
	}
}

void MyReflexTree::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
#ifdef _SHOW_BOUNDINGBOX
	m_MyBOBox->Render(pd3dCommandList, Matrix4x4::Identity());
#endif // _SHOW_BOUNDINGBOX

	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("InstancingStandardShader")->GetPSO());
	UpdateShaderVariables(pd3dCommandList, m_CountFromMap);
	m_LoadObject->RenderInstancing(pd3dCommandList, m_CountFromMap);
}
//// MyReflexTree /////////////////////////////////////////////////////////////
 

//// SunFlower /////////////////////////////////////////////////////////////
std::vector<XMFLOAT4X4> SunFlower::m_PostionFromMap(2000);
int SunFlower::m_CountFromMap = 0;
int SunFlower::m_Count = 0;

SunFlower::SunFlower(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("SunFlower") 
{
}

SunFlower::SunFlower(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: StaticObject("SunFlower") 
{
	ModelStorage::GetInstance()->CreateModels(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	m_LoadObject = ModelStorage::GetInstance()->GetRootObject("SunFlower");

	XMFLOAT4X4 transform = m_PostionFromMap[m_Count];
	XMFLOAT3 position =
	{
		-transform._41,
		transform._42,
		-transform._43,
	};

	m_LoadObject->SetPosition(position);

	m_Count += 1;

	XMFLOAT3 center{ -30.f, 100.f, 0.f };
	XMFLOAT3 extents{ 100.f, 100.f, 100.f };

	center = Vector3::Add(center, position);
	m_MyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
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
std::vector<XMFLOAT4X4> Rock::m_PostionFromMap(1000);
int Rock::m_CountFromMap = 0;
int Rock::m_Count = 0;

Rock::Rock(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("Rock") 
{
}

Rock::Rock(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: StaticObject("Rock") 
{
	ModelStorage::GetInstance()->CreateModels(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	m_LoadObject = ModelStorage::GetInstance()->GetRootObject("Rock");

	XMFLOAT4X4 transform = m_PostionFromMap[m_Count];

	XMFLOAT3 position =
	{
		-transform._41,
		transform._42,
		-transform._43,
	};

	m_LoadObject->SetPosition(position);

	m_LoadObject->UpdateTransform(NULL);
	m_Count += 1;

	XMFLOAT3 center{ -30.f, 100.f, 0.f };
	XMFLOAT3 extents{ 100.f, 100.f, 100.f };

	center = Vector3::Add(center, position);
	m_MyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
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
std::vector<XMFLOAT4X4> Pillar::m_PostionFromMap(1000);
int Pillar::m_CountFromMap = 0;
int Pillar::m_Count = 0;

Pillar::Pillar(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("Pilla")
{
}

Pillar::Pillar(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: StaticObject("Pillar")
{
	ModelStorage::GetInstance()->CreateModels(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	m_LoadObject = ModelStorage::GetInstance()->GetRootObject("Pillar");

	XMFLOAT4X4 transform = m_PostionFromMap[m_Count];

	XMFLOAT3 position =
	{
		-transform._41,
		transform._42,
		-transform._43,
	};

	m_LoadObject->SetPosition(position);

	m_LoadObject->UpdateTransform(NULL);
	m_Count += 1;

	XMFLOAT3 center{ -30.f, 100.f, 0.f };
	XMFLOAT3 extents{ 100.f, 100.f, 100.f };

	center = Vector3::Add(center, position);
	m_MyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
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
std::vector<XMFLOAT4X4> Altar::m_PostionFromMap(50);
int Altar::m_CountFromMap = 0;
int Altar::m_Count = 0;

Altar::Altar(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, XMFLOAT3 position)
	: StaticObject("Altar") 
{
}

Altar::Altar(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: StaticObject("Altar") 
{
	ModelStorage::GetInstance()->CreateModels(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	m_LoadObject = ModelStorage::GetInstance()->GetRootObject("Altar");

	XMFLOAT4X4 transform = m_PostionFromMap[m_Count];

	XMFLOAT3 position =
	{
		-transform._41,
		transform._42,
		-transform._43,
	};

	m_LoadObject->SetPosition(position);

	m_LoadObject->UpdateTransform(NULL);
	m_Count += 1;

	XMFLOAT3 center{ -30.f, 100.f, 0.f };
	XMFLOAT3 extents{ 100.f, 100.f, 100.f };

	center = Vector3::Add(center, position);
	m_MyBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, center, extents);
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
