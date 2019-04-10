#include "stdafx.h"
#include "MyRectangle.h"
#include "Button.h"

void Button::ReleaseMembers()
{
	
}

void Button::ReleaseMemberUploadBuffers()
{
}

Button::Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect)
	:GameObject(entityID)
{
	m_pButton = new MyRectangle(this, pd3dDevice, pd3dCommandList, rect);
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
