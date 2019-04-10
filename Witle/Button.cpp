#include "stdafx.h"
#include "Texture.h"
#include "MyRectangle.h"
#include "Button.h"

void Button::ReleaseMembers()
{
	
}

void Button::ReleaseMemberUploadBuffers()
{
}

Button::Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath)
	:GameObject(entityID)
{
	m_pButton = new MyRectangle(this, pd3dDevice, pd3dCommandList, rect, filepath);
}

Button::~Button()
{

}

void Button::Update(float fElapsedTime)
{

}

void Button::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_pButton->Render(pd3dCommandList);
}

Texture * Button::GetTexture()
{
	return m_pButton->GetTexture();
}
