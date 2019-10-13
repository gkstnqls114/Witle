#include "stdafx.h"
#include "TextureStorage.h"
#include "UI2DChangedImage.h"

UI2DChangedImage::UI2DChangedImage(GameObject * pOwner, ENUM_SCENE Scenetype, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, RECT rect, const std::string & FalseFilepath, const std::string & TrueFilepath)
	: UI2DImage(pOwner, Scenetype, pd3dDevice, pd3dCommandList, rect, "")
{ 
	m_pFalseTexture = TextureStorage::GetInstance()->GetTexture(FalseFilepath);
	m_pTrueTexture = TextureStorage::GetInstance()->GetTexture(TrueFilepath);
	Init();
}

UI2DChangedImage::UI2DChangedImage(GameObject * pOwner, ENUM_SCENE Scenetype, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, POINT center, float width, float height, const std::string & FalseFilepath, const std::string & TrueFilepath)
	: UI2DImage(pOwner, Scenetype, pd3dDevice, pd3dCommandList, center, width, height, "")
{
	m_pFalseTexture = TextureStorage::GetInstance()->GetTexture(FalseFilepath);
	m_pTrueTexture = TextureStorage::GetInstance()->GetTexture(TrueFilepath);
	Init();
}

void UI2DChangedImage::Init()
{
	isState = false;
	SetpTexture(m_pFalseTexture);
}

void UI2DChangedImage::ChangeFalse()
{
	isState = false;
	SetpTexture(m_pFalseTexture);
}

void UI2DChangedImage::ChangeTrue()
{
	isState = true;
	SetpTexture(m_pTrueTexture);
}
