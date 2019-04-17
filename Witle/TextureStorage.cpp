#include "stdafx.h"
#include "Texture.h" 
#include "TextureStorage.h"

TextureStorage* TextureStorage::m_Instance{ nullptr };

TextureStorage::TextureStorage()
{

}

TextureStorage::~TextureStorage()
{

}

void TextureStorage::ReleaseUploadBuffers()
{
	for (auto& texture : m_TextureStorage)
	{
		texture.second->ReleaseUploadBuffers();
	}
}

void TextureStorage::ReleaseObjects()
{
	for (auto& texture : m_TextureStorage)
	{
		delete texture.second;
		texture.second = nullptr;
	}
	m_TextureStorage.clear();
}

void TextureStorage::CreateTextures(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{

}

void TextureStorage::InsertTexture(const std::string& name, Texture * texture)
{
	m_TextureStorage[name] = texture;
}

const Texture * TextureStorage::GetTexture(const std::string & name)
{
	return m_TextureStorage[name];
}
 