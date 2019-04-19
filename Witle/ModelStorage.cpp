#include "stdafx.h"
#include "Object.h"  
#include "MyBOBox.h"
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
	// m_NameList.push_back(TREE_1);
	//m_NameList.push_back(TREE_2);
	//m_NameList.push_back(TREE_3);
	//m_NameList.push_back(BUSH);
	//m_NameList.push_back(BROKENPILLA);
	//m_NameList.push_back(REED);
	//m_NameList.push_back(RUIN_ARCH);
	//m_NameList.push_back(RUIN_BROKENPILLA);
	//m_NameList.push_back(RUIN_PILLAR);
	//m_NameList.push_back(RUIN_SQUARE);
	//m_NameList.push_back(SUNFLOWER);
	 
	// 모델 목록
	// m_ModelStorage[TREE_1].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/TreeOne.bin", NULL);
	//m_ModelStorage[TREE_2].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/TreeTwo.bin", NULL);
	//m_ModelStorage[TREE_3].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/TreeThree.bin", NULL);
	//m_ModelStorage[REED].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Bush.bin", NULL);
	//m_ModelStorage[BROKENPILLA].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/BrokenPilla.bin", NULL);
	//m_ModelStorage[BUSH].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Reed.bin", NULL);
	//m_ModelStorage[RUIN_ARCH].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile
	//(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ruin_Arch.bin", NULL);
	//m_ModelStorage[RUIN_BROKENPILLA].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile
	//(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ruin_BrokenPillar.bin", NULL);
	//m_ModelStorage[RUIN_PILLAR].loadmodelInfo =
	//	LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ruin_Pillar.bin", NULL);
	//m_ModelStorage[RUIN_SQUARE].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ruin_Square.bin", NULL);
	//m_ModelStorage[SUNFLOWER].loadmodelInfo = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Sunflower.bin", NULL);
	
	// 충돌박스 목록
	// m_ModelStorage[TREE_1].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{0.F, 0.F, 0.F}, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[TREE_2].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[TREE_3].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[BUSH].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[BROKENPILLA].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[REED].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[RUIN_ARCH].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[RUIN_BROKENPILLA].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[RUIN_PILLAR].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[RUIN_SQUARE].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });
	//m_ModelStorage[SUNFLOWER].modelBOBox = new MyBOBox(pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, XMFLOAT3{ 400.F, 400.F, 400.F });

	m_isCreate = true;
}

void ModelStorage::ReleaseUploadBuffers()
{
	for (auto& model : m_ModelStorage)
	{
		 model.second.loadmodelInfo->m_pModelRootObject->ReleaseUploadBuffers();
		 model.second.modelBOBox->ReleaseUploadBuffers();
	} 
}

void ModelStorage::ReleaseObjects()
{ 
	for (auto& model : m_ModelStorage)
	{
		// model.second.loadmodelInfo->ReleaseObjects();
		delete model.second.loadmodelInfo;
		model.second.loadmodelInfo = nullptr; 
		  
		// model.second.modelBOBox->ReleaseObjects();
		delete model.second.modelBOBox;
		model.second.modelBOBox = nullptr;
	}

	m_ModelStorage.clear(); 
	m_NameList.clear();
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
