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
  
 