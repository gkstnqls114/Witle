#include "stdafx.h"
#include "Object.h" 
#include "MyBOBox.h"
#include "ModelStorage.h"

ModelStorage* ModelStorage::m_Instance{ nullptr };

ModelStorage * ModelStorage::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new ModelStorage();
	}

	return m_Instance;
}

void ModelStorage::CreateModels(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	if (m_isCreate) return;

	// 이름목록
	m_NameList.push_back("ReflexTree");
	m_NameList.push_back("Rock");
	m_NameList.push_back("Pillar");
	m_NameList.push_back("Sunflower");

	// 모델 목록
	m_ModelStorage["ReflexTree"].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ReflexTree.bin", NULL);
	m_ModelStorage["Rock"].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Rock.bin", NULL);
	m_ModelStorage["Pillar"].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Pillar.bin", NULL);
	m_ModelStorage["Sunflower"].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Sunflower.bin", NULL);
	 
	// 충돌박스 목록
	m_ModelStorage["ReflexTree"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{0.F, 0.F, 0.F}, XMFLOAT3{ 10.F, 10.F, 10.F });
	m_ModelStorage["Rock"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 20.F, 20.F, 20.F });
	m_ModelStorage["Pillar"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 30.F, 30.F, 30.F });
	m_ModelStorage["Sunflower"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 40.F, 40.F, 40.F });

	m_isCreate = true;
}

LoadObject * ModelStorage::GetRootObject(std::string name)
{
	if (!m_ModelStorage[name].loadmodelInfo) return nullptr;

	LoadObject* newRootObject = new LoadObject;
	LoadObject::CopyWorldMatrix(newRootObject, m_ModelStorage[name].loadmodelInfo->m_pModelRootObject);

	return newRootObject;
}

MyBOBox * ModelStorage::GetBOBox(std::string name)
{ 
	if (!m_ModelStorage[name].modelBOBox) return nullptr;
	 
	return m_ModelStorage[name].modelBOBox;
}
