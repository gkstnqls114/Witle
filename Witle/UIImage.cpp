#include "stdafx.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "UIImage.h"
 
void UIImage::ReleaseObjects()
{
}

void UIImage::ReleaseUploadBuffers()
{
}

UIImage::UIImage(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, RECT rect, const wchar_t * filepath)
	:UI(pOwner)
{  
	if (filepath)
	{
		m_pHeap = new MyDescriptorHeap();
		m_pHeap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
		m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
		m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filepath, 0);
		m_pHeap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, true);
	}
}

UIImage::UIImage(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath)
	: UI(pOwner)
{
	if (filepath)
	{
		m_pHeap = new MyDescriptorHeap();
		m_pHeap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
		m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
		m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filepath, 0);
		m_pHeap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, true);
	}
}

UIImage::~UIImage()
{

}
 
void UIImage::Update(float fElapsedTime)
{

}

void UIImage::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{

}
