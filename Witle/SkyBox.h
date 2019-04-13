#pragma once
#include "GameObject.h"

class LoadObject;

class SkyBox
	: public GameObject
{ 
	LoadObject* m_LoadObject{ nullptr };

protected:
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ��������� �����Ѵ�.
	virtual void ReleaseMembers() = 0;
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ���ε� ���� �����Ѵ�.
	virtual void ReleaseMemberUploadBuffers() = 0;

public:
	// GameObject::Update�� ��� ������ �Ļ��� ���� ������Ʈ���� �ٸ���.
	virtual void Update(float fElapsedTime) = 0;

public:
	SkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~SkyBox();
};