#include "stdafx.h"
#include "Texture.h"
#include "MyRectangle.h"
#include "Widget.h"

void Widget::ReleaseMembers()
{

}

void Widget::ReleaseMemberUploadBuffers()
{
}

Widget::Widget(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath)
	:GameObject(entityID)
{
	m_rect = rect;
	m_AimPoint = new MyRectangle(this, pd3dDevice, pd3dCommandList, rect, filepath); 
}

Widget::Widget(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath)
	:GameObject(entityID)
{
	m_rect.left = static_cast<float>(center.x) - width / 2.f;
	m_rect.right = static_cast<float>(center.x) + width / 2.f;
	m_rect.top = static_cast<float>(center.y) - height / 2.f;
	m_rect.bottom = static_cast<float>(center.y) + height / 2.f;

	m_AimPoint = new MyRectangle(this, pd3dDevice, pd3dCommandList, center, width, height, filepath);
}
Widget::~Widget()
{

}
 
void Widget::Update(float fElapsedTime)
{

}

void Widget::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_AimPoint->Render(pd3dCommandList);
}

Texture * Widget::GetTexture()
{ 
	return m_AimPoint->GetTexture(); 
}
