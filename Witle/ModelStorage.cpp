#include "stdafx.h"
#include "Object.h"  
#include "MyBOBox.h"
#include "TextureStorage.h"
#include "ModelStorage.h"

ModelStorage* ModelStorage::m_Instance{ nullptr };

ModelStorage * ModelStorage::GetInstance()
{
	if(nullptr == m_Instance)
	{
		m_Instance = new ModelStorage();
	}

	return m_Instance;
}

void ModelStorage::ReleaseInstance()
{
	m_Instance->ReleaseObjects();
	delete m_Instance;
	m_Instance = nullptr;
}
 
void ModelStorage::CreateModels(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	if (m_isCreate) return;

	// 이름목록
	m_NameList.push_back(ALTAR_IN);
	m_NameList.push_back(ALTAR_OUT);
	m_NameList.push_back(BUSH);
	m_NameList.push_back(BUSHSQUARE);
	// m_NameList.push_back(Cliff);
	// m_NameList.push_back(Flower);
	m_NameList.push_back(REED);
	m_NameList.push_back(RUIN_ARCH);
	m_NameList.push_back(RUIN_BROKENPILLA);
	m_NameList.push_back(RUIN_PILLAR);
	m_NameList.push_back(RUIN_SQUARE);
	m_NameList.push_back(SUNFLOWER); 
	m_NameList.push_back(TREE_1);
	m_NameList.push_back(TREE_BG_1);
	m_NameList.push_back(TREE_2);
	m_NameList.push_back(TREE_BG_2);
	m_NameList.push_back(TREE_3);
	m_NameList.push_back(TREE_BG_3); 
	 
	 // 모델 목록 
	for (const auto& name : m_NameList)
	{
		std::string path = "Model/";
		path.append(name);
		path.append(".bin");
		m_ModelStorage[name].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, path.c_str(), NULL);
	}

	// 충돌박스 목록
	m_ModelStorage[TREE_1].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 200.F, 200.F, 200.F });
	m_ModelStorage[TREE_BG_1].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{0.F, 0.F, 0.F}, XMFLOAT3{ 200.F, 200.F, 200.F });
	m_ModelStorage[TREE_2].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 100.F, 100.F, 100.F });
	m_ModelStorage[TREE_BG_2].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{0.F, 0.F, 0.F},XMFLOAT3{ 200.F, 200.F, 200.F });
	m_ModelStorage[TREE_3].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 100.F, 100.F, 100.F });
	m_ModelStorage[TREE_BG_3].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{0.F, 0.F, 0.F},XMFLOAT3{ 200.F, 200.F, 200.F });
	m_ModelStorage[RUIN_ARCH].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 300.F, 300.F, 300.F });
	m_ModelStorage[RUIN_BROKENPILLA].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 300.F, 300.F, 300.F });
	m_ModelStorage[RUIN_PILLAR].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	m_ModelStorage[RUIN_SQUARE].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	m_ModelStorage[ALTAR_IN].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 200.F, 200.F, 200.F });
	// m_ModelStorage[ALTAR_IN].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 200.F, 200.F, 200.F });
	// m_ModelStorage[SUNFLOWER].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	 
	m_isCreate = true;
}

void ModelStorage::ReleaseUploadBuffers()
{
	for (auto& model : m_ModelStorage)
	{
		if(model.second.loadmodelInfo) model.second.loadmodelInfo->m_pModelRootObject->ReleaseUploadBuffers();
		if(model.second.modelBOBox) model.second.modelBOBox->ReleaseUploadBuffers();
	} 
}

void ModelStorage::ReleaseObjects()
{ 
	for (auto& model : m_ModelStorage)
	{
		model.second.loadmodelInfo->ReleaseObjects();
		delete model.second.loadmodelInfo;
		model.second.loadmodelInfo = nullptr; 
		  
		if (model.second.modelBOBox)
		{ 
			model.second.modelBOBox->ReleaseObjects();
			delete model.second.modelBOBox;
			model.second.modelBOBox = nullptr;
		}
	}

	m_ModelStorage.clear(); 
	m_NameList.clear();
}

LoadObject * ModelStorage::GetRootObject(std::string name)
{
	if (!m_ModelStorage[name].loadmodelInfo) return nullptr;

	LoadObject* newRootObject = new LoadObject(0);
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
	if (!m_ModelStorage[name].modelBOBox) return;

	m_ModelStorage[name].modelBOBox->RenderInstancing(pd3dCommandList, InstancingCount);
}
#endif // _SHOW_BOUNDINGBOX
