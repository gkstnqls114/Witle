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
	m_ModelStorage["ReflexTree"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{0.F, 0.F, 0.F}, XMFLOAT3{ 400.F, 400.F, 400.F });
	m_ModelStorage["Rock"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	m_ModelStorage["Pillar"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	m_ModelStorage["Sunflower"].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });

	m_isCreate = true;
}

void ModelStorage::ReleaseUploadBuffers()
{
	for (auto& model : m_ModelStorage)
	{
		model.second.loadmodelInfo->m_pModelRootObject->ReleaseUploadBuffers();
	}
	for (auto& model : m_ModelStorage)
	{
		model.second.modelBOBox->ReleaseUploadBuffers();
	}
}

void ModelStorage::ReleaseObjects()
{
	for (auto& model : m_ModelStorage)
	{
		delete model.second.loadmodelInfo;
		model.second.loadmodelInfo = nullptr;
	}
	for (auto& model : m_ModelStorage)
	{
		delete model.second.modelBOBox;
		model.second.modelBOBox = nullptr;
	}
}

LoadObject * ModelStorage::GetRootObject(std::string name)
{
	if (!m_ModelStorage[name].loadmodelInfo) return nullptr;

	LoadObject* newRootObject = new LoadObject(1);
	LoadObject::CopyWorldMatrix(newRootObject, m_ModelStorage[name].loadmodelInfo->m_pModelRootObject);

	return newRootObject;
}

MyBOBox * ModelStorage::GetBOBox(std::string name)
{ 
	if (!m_ModelStorage[name].modelBOBox) return nullptr;
	 
	return m_ModelStorage[name].modelBOBox;
}
#ifdef _SHOW_BOUNDINGBOX
void ModelStorage::RenderBOBoxInstancing(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name, int InstancingCount)
{
	m_ModelStorage[name].modelBOBox->RenderInstancing(pd3dCommandList, InstancingCount);
}
#endif // _SHOW_BOUNDINGBOX
