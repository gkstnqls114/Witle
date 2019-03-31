#include "stdafx.h"
#include "Object.h" 
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

	m_ModelStorage["ReflexTree"] = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ReflexTree.bin", NULL);
	m_isCreate = true;
}

LoadObject * ModelStorage::GetRootObject(std::string name)
{
	LoadObject* newRootObject = new LoadObject;
	LoadObject::CopyWorldMatrix(newRootObject, m_ModelStorage[name]->m_pModelRootObject);

	return newRootObject;
}
