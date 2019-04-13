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

protected:
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 멤버변수를 해제한다.
	virtual void ReleaseMembers();
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 업로드 힙을 해제한다.
	virtual void ReleaseMemberUploadBuffers();

public:
	// GameObject::Update의 모든 구현은 파생된 게임 오브젝트마다 다르다.
	virtual void Update(float fElapsedTime) { };

public:
	SkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~SkyBox();

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};