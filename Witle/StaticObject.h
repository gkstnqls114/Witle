#pragma once
#include "GameObject.h"

class StaticObject
	:public GameObject
{ 
protected:
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 멤버변수를 해제한다.
	virtual void ReleaseMembers() override;
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 업로드 힙을 해제한다.
	virtual void ReleaseMemberUploadBuffers() override;

public:
	// GameObject::Update의 모든 구현은 파생된 게임 오브젝트마다 다르다.
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
