#pragma once
#include "GameObject.h"

class MyRectangle;

class Button
	: public GameObject
{
	MyRectangle* m_pButton{ nullptr };

private:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect);
	virtual ~Button();

	virtual void Update(float fElapsedTime) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};