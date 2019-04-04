#pragma once
#include "GameObject.h"

class StaticObject
	:public GameObject
{ 
protected:
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ��������� �����Ѵ�.
	virtual void ReleaseMembers() override;
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ���ε� ���� �����Ѵ�.
	virtual void ReleaseMemberUploadBuffers() override;

public:
	// GameObject::Update�� ��� ������ �Ļ��� ���� ������Ʈ���� �ٸ���.
	virtual void Update(float fElapsedTime) override {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int nInstance) {};
	 
public:
	StaticObject(const std::string &EntityID);
	virtual ~StaticObject() {};
};
 
class LoadObject;
class MyBOBox;

struct VS_SRV_INSTANCEINFO
{
	XMFLOAT4X4 m_xmf4x4Transform;
};

class MyReflexTree : public StaticObject
{ 
	// �ν��Ͻ� ������
	static ID3D12Resource* m_pd3dcbGameObjects;
	static VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects;
	// �ν��Ͻ� ������

	MyBOBox* m_MyBOBox{ nullptr };
	LoadObject* m_LoadObject{ nullptr };

public:
	MyReflexTree(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, XMFLOAT3 position);
	MyReflexTree(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~MyReflexTree() {};

	static void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int nInstance) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	MyBOBox* GetBOBox() { return m_MyBOBox;  }
	 
	static std::vector<XMFLOAT4X4> m_PostionFromMap;
	static int m_CountFromMap;
	static int m_Count;
	
};

class SunFlower : public StaticObject
{ 
	MyBOBox* m_MyBOBox{ nullptr };
	LoadObject* m_LoadObject{ nullptr };

public:
	SunFlower(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, XMFLOAT3 position);
	SunFlower(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~SunFlower() {};

 	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	MyBOBox* GetBOBox() { return m_MyBOBox; }

	static std::vector<XMFLOAT4X4> m_PostionFromMap;
	static int m_CountFromMap;
	static int m_Count;

};

class Rock : public StaticObject
{  
	MyBOBox* m_MyBOBox{ nullptr };
	LoadObject* m_LoadObject{ nullptr };

public:
	Rock(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, XMFLOAT3 position);
	Rock(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Rock() {};

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	MyBOBox* GetBOBox() { return m_MyBOBox; }

	static std::vector<XMFLOAT4X4> m_PostionFromMap;
	static int m_CountFromMap;
	static int m_Count;

};

class Pillar : public StaticObject
{ 
	MyBOBox* m_MyBOBox{ nullptr };
	LoadObject* m_LoadObject{ nullptr };

public:
	Pillar(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, XMFLOAT3 position);
	Pillar(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Pillar() {};

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	MyBOBox* GetBOBox() { return m_MyBOBox; }

	static std::vector<XMFLOAT4X4> m_PostionFromMap;
	static int m_CountFromMap;
	static int m_Count;

};


class Altar : public StaticObject
{
	// �ν��Ͻ� ������
	static ID3D12Resource* m_pd3dcbGameObjects;
	static VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects;
	// �ν��Ͻ� ������

	MyBOBox* m_MyBOBox{ nullptr };
	LoadObject* m_LoadObject{ nullptr };

public:
	Altar(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, XMFLOAT3 position);
	Altar(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Altar() {};

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	MyBOBox* GetBOBox() { return m_MyBOBox; }

	static std::vector<XMFLOAT4X4> m_PostionFromMap;
	static int m_CountFromMap;
	static int m_Count;

};