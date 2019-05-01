#pragma once
#include "GameObject.h"

class LoadObject;
class MyDescriptorHeap;
class Texture;

class SkyBox
	: public GameObject
{ 
	MyDescriptorHeap * m_Heap { nullptr };
	Texture * m_Texture{ nullptr };
	LoadObject* m_LoadObject  { nullptr };

	Transform* m_pPlayerTransform{ nullptr };

protected: 
	virtual void ReleaseMembers(); 
	virtual void ReleaseMemberUploadBuffers();

public:
	// GameObject::Update�� ��� ������ �Ļ��� ���� ������Ʈ���� �ٸ���.
	virtual void Update(float fElapsedTime);

public:
	SkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~SkyBox();
	 
	void SetpPlayerTransform(Transform* tr) { m_pPlayerTransform = tr; }
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};