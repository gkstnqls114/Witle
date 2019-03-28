#pragma once

class CLoadedModelInfo;
class CMesh;
class LoadObject;

class ModelStorage
{
	bool m_isCreate = false;
	std::map<std::string, CLoadedModelInfo*> m_ModelStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	static ModelStorage* m_Instance;

public:
	static ModelStorage* GetInstance();

	void CreateModels(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature);
	LoadObject * GetRootObject(std::string name); 
};