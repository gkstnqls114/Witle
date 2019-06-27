#include "stdafx.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "MyRectangle.h"
#include "Button.h"

void Button::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	if (m_isClick)
	{
		m_pButtonON->Render(pd3dCommandList, m_pButtonShader);
	}
	else
	{
		m_pButtonOFF->Render(pd3dCommandList, m_pButtonShader);
	}
}

void Button::ReleaseMembers()
{
	
}

void Button::ReleaseMemberUploadBuffers()
{
}

Button::Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * ON_filepath, const wchar_t * OFF_filepath)
	:GameObject(entityID)
{ 
	m_pButtonShader = ShaderManager::GetInstance()->GetShader(SHADER_SCREEN);

	m_rect = rect;
	m_pButtonON = new MyRectangle(this, pd3dDevice, pd3dCommandList, rect, ON_filepath);
	m_pButtonOFF = new MyRectangle(this, pd3dDevice, pd3dCommandList, rect, OFF_filepath);
}
Button::Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * ON_filepath, const wchar_t * OFF_filepath)
	:GameObject(entityID)
{ 
	m_pButtonShader = ShaderManager::GetInstance()->GetShader(SHADER_SCREEN);

	m_rect.left = static_cast<float>(center.x) - width / 2.f;
	m_rect.right = static_cast<float>(center.x) + width / 2.f;
	m_rect.top = static_cast<float>(center.y) - height / 2.f;
	m_rect.bottom =static_cast<float>(center.y) + height / 2.f;
	
	m_pButtonON = new MyRectangle(this, pd3dDevice, pd3dCommandList, center, width, height, ON_filepath);
	m_pButtonOFF = new MyRectangle(this, pd3dDevice, pd3dCommandList, center, width, height, OFF_filepath);
}
Button::~Button()
{

}

bool Button::CheckClick(POINT clickcursor)
{
	if (clickcursor.x < m_rect.left)return false;
	if (clickcursor.x > m_rect.right)return false;
	if (clickcursor.y < m_rect.top)return false;
	if (clickcursor.y > m_rect.bottom)return false;
	m_isClick = !m_isClick;
	return true;
}

bool Button::CheckClickTRUE(POINT clickcursor)
{
	if (clickcursor.x < m_rect.left)return false;
	if (clickcursor.x > m_rect.right)return false;
	if (clickcursor.y < m_rect.top)return false;
	if (clickcursor.y > m_rect.bottom)return false;
	m_isClick = true;
	return true;
}

void Button::Update(float fElapsedTime)
{

}
 
Texture * Button::GetTexture(bool isON)
{
	if (isON)
	{
		return m_pButtonON->GetTexture();
	}
	else
	{ 
		return m_pButtonOFF->GetTexture();
	}
}
