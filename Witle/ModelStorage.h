#pragma once

class CLoadedModelInfo;
class CMesh;
class MyBOBox;
class LoadObject;

class ModelStorage
{
	bool m_isCreate = false;
	
	struct ModelInfo
	{
		CLoadedModelInfo* loadmodelInfo{ nullptr };
		MyBOBox*          modelBOBox{ nullptr };
	};

	std::map<std::string, ModelInfo> m_ModelStorage; // �� �̸��� �ݵ�� Ŭ������ �����.

	static ModelStorage* m_Instance;

public:
	std::list<std::string> m_NameList;
public:
	static ModelStorage* GetInstance();

	void CreateModels(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature);
	LoadObject * GetRootObject(std::string name);
	MyBOBox * GetBOBox(std::string name);
#ifdef _SHOW_BOUNDINGBOX
	void RenderBOBoxInstancing(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name, int InstancingCount);
#endif // _SHOW_BOUNDINGBOX

};