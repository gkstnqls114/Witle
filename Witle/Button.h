#pragma once
#include "GameObject.h"

class MyRectangle; 
class Texture;

class Button
	: public GameObject
{
	MyRectangle* m_pButton{ nullptr }; 
	bool m_isClick{ false };

private:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	virtual ~Button();

	virtual void Update(float fElapsedTime) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	Texture* GetTexture();
};