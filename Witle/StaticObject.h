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
	 
public:
	StaticObject(const std::string &EntityID);
	virtual ~StaticObject() {};
};
 
class LoadObject;
class MyBOBox;

class MyReflexTree : public StaticObject
{
	MyBOBox* m_MyBOBox{ nullptr };
	LoadObject* m_LoadObject{ nullptr };

public:
	MyReflexTree(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, XMFLOAT3 position);
	virtual ~MyReflexTree() {};
	 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	MyBOBox* GetBOBox() {
		return m_MyBOBox
			;
	}
};
