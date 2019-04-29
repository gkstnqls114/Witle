#include "stdafx.h"
#include "Texture.h" 
#include "MyDescriptorHeap.h"
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
	m_Heap = new MyDescriptorHeap();
	m_Heap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 3, 0);
	int heapIndex = 0;

	m_TextureStorage[TREE_1] = new Texture();
	m_TextureStorage[TREE_1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/TreeOne_Diffuse.dds", 0);
	m_Heap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_TextureStorage[TREE_1], ROOTPARAMETER_TEXTURE, RESOURCE_TEXTURE2D, heapIndex++);
	
	m_TextureStorage[TREE_2] = new Texture();
	m_TextureStorage[TREE_2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/TreeTwo_Diffuse.dds", 0);
	m_Heap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_TextureStorage[TREE_2], ROOTPARAMETER_TEXTURE, RESOURCE_TEXTURE2D, heapIndex++);

	m_TextureStorage[TREE_3] = new Texture();
	m_TextureStorage[TREE_3]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/TreeThree_Diffuse.dds", 0);
	m_Heap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_TextureStorage[TREE_3], ROOTPARAMETER_TEXTURE, RESOURCE_TEXTURE2D, heapIndex++);

}
 
Texture * const TextureStorage::GetTexture(const std::string & name)
{
	if (m_TextureStorage.find(name) == m_TextureStorage.end()) return nullptr;
	return m_TextureStorage[name];
}

void TextureStorage::SetHeap(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_Heap->UpdateShaderVariable(pd3dCommandList);
}
 