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

void TextureStorage::CreateTexture(const std::string & name, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int heapindex)
{
	m_TextureStorage[name] = new Texture();
	std::wstring path = { L"Model/Textures/" };
	std::wstring wname;
	wname.assign(name.begin(), name.end()); 
	path.append(wname);
	path.append(L".dds");
	m_TextureStorage[name]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, path.c_str(), 0);
	m_Heap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_TextureStorage[name], ROOTPARAMETER_TEXTURE, RESOURCE_TEXTURE2D, heapindex);
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
	if (m_Heap)
	{
		m_Heap->ReleaseObjects();

		delete m_Heap;
		m_Heap = nullptr;
	}
	
	for (auto& texture : m_TextureStorage)
	{
		if (texture.first == TREE_BG_1) continue;
		if (texture.first == TREE_BG_2) continue;
		if (texture.first == TREE_BG_3) continue;
		if (texture.first == BUSHSQUARE) continue;
		delete texture.second;
		texture.second = nullptr;
	}
	m_TextureStorage.clear();
}

void TextureStorage::CreateTextures(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_Heap = new MyDescriptorHeap();
	m_Heap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 18, 0);
	int heapIndex = 0;
	CreateTexture(ALTAR_IN, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(ALTAR_OUT, pd3dDevice, pd3dCommandList, heapIndex++);

	CreateTexture(BUSH, pd3dDevice, pd3dCommandList, heapIndex++);
	m_TextureStorage[BUSHSQUARE] = m_TextureStorage[BUSH];
	// CreateTexture(BUSHSQUARE, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(Cliff, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(Flower, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(REED, pd3dDevice, pd3dCommandList, heapIndex++);

	CreateTexture(RUIN_ARCH, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(RUIN_BROKENPILLA, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(RUIN_PILLAR, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(RUIN_SQUARE, pd3dDevice, pd3dCommandList, heapIndex++);

	CreateTexture(SUNFLOWER, pd3dDevice, pd3dCommandList, heapIndex++);

	CreateTexture(TREE_1, pd3dDevice, pd3dCommandList, heapIndex++);
	m_TextureStorage[TREE_BG_1] = m_TextureStorage[TREE_1];
	//CreateTexture(TREE_BG_1, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(TREE_3, pd3dDevice, pd3dCommandList, heapIndex++);
	m_TextureStorage[TREE_BG_3] = m_TextureStorage[TREE_3];
	// CreateTexture(TREE_BG_3, pd3dDevice, pd3dCommandList, heapIndex++);
	CreateTexture(TREE_2, pd3dDevice, pd3dCommandList, heapIndex++);
	m_TextureStorage[TREE_BG_2] = m_TextureStorage[TREE_2];
	// CreateTexture(TREE_BG_2, pd3dDevice, pd3dCommandList, heapIndex++);
	 
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
 