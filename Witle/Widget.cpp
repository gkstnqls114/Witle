#include "stdafx.h"
#include "Texture.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "Widget.h"

void Widget::ReleaseMembers()
{
	if (m_AimPoint)
	{
		m_AimPoint->ReleaseObjects();
		delete m_AimPoint;
		m_AimPoint = nullptr;
	}
}

void Widget::ReleaseMemberUploadBuffers()
{
	if(m_AimPoint) m_AimPoint->ReleaseUploadBuffers();
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

AimPoint::AimPoint(const std::string & entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath)
	: Widget(entityID, pd3dDevice, pd3dCommandList, POINT{0, 0}, width, height, filepath)
{
	m_PickingPoint = XMFLOAT2{ GameScreen::GetWidth() / 2 , GameScreen::GetHeight() /2};
}

AimPoint::~AimPoint()
{
}

//// AimPoint /////////////////////////////////////////////////
void AimPoint::MovePickingPoint(const XMFLOAT2 & shift)
{
	m_PickingPoint = Vector2::Add(m_PickingPoint, shift);
}

void AimPoint::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 피킹 포인트 위해 설정
	m_AimPoint->Render(pd3dCommandList, m_PickingPoint, 0.f);
}
