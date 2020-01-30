#pragma once

class CLoadedModelInfo;
class CMesh;
class MyBOBox;
class LoadObject;
class Texture;
class EmptyGameObject;
class CAnimationController;

class ModelStorage
{
	bool m_isCreate{ false };
	
	struct ModelInfo
	{
		CLoadedModelInfo* loadmodelInfo{ nullptr };
		MyBOBox*          modelBOBox{ nullptr }; 
	};

	std::map<std::string, ModelInfo> m_ModelStorage; // 모델 이름은 반드시 클래스에 맞춘다.

	static EmptyGameObject* test; 
	static ModelStorage* m_Instance;

public:
	std::list<std::string> m_NameList;
public:
	static ModelStorage* GetInstance();
	static void ReleaseInstance();

	void CreateModels(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature);
	void ReleaseUploadBuffers();
	void ReleaseObjects();

	LoadObject* GetRootObject(std::string name); 
	MyBOBox * GetBOBox(std::string name);
	CLoadedModelInfo * GetModelInfo(std::string name);

	void RenderBOBoxInstancing(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name, int InstancingCount);

	void Render(ID3D12GraphicsCommandList * pd3dCommandList, const std::string& name, CAnimationController* Controller);
};