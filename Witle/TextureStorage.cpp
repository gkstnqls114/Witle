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

void TextureStorage::CreateTexture(const std::string & name, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_TextureStorage[name] = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	std::wstring path = { L"Model/Textures/" };
	std::wstring wname;
	wname.assign(name.begin(), name.end()); 
	path.append(wname);
	path.append(L".dds");
	m_TextureStorage[name]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, path.c_str(), 0);
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
	// 터레인 오브젝트 들의 텍스쳐 미리 로드
	CreateTexture(ALTAR_IN, pd3dDevice, pd3dCommandList);
	CreateTexture(ALTAR_OUT, pd3dDevice, pd3dCommandList); 
	CreateTexture(BUSH, pd3dDevice, pd3dCommandList);
	m_TextureStorage[BUSHSQUARE] = m_TextureStorage[BUSH]; 
	CreateTexture(Cliff, pd3dDevice, pd3dCommandList);
	CreateTexture(Flower, pd3dDevice, pd3dCommandList);
	CreateTexture(REED, pd3dDevice, pd3dCommandList); 
	CreateTexture(RUIN_ARCH, pd3dDevice, pd3dCommandList);
	CreateTexture(RUIN_BROKENPILLA, pd3dDevice, pd3dCommandList);
	CreateTexture(RUIN_PILLAR, pd3dDevice, pd3dCommandList);
	CreateTexture(RUIN_SQUARE, pd3dDevice, pd3dCommandList); 
	CreateTexture(SUNFLOWER, pd3dDevice, pd3dCommandList); 
	CreateTexture(TREE_1, pd3dDevice, pd3dCommandList);
	m_TextureStorage[TREE_BG_1] = m_TextureStorage[TREE_1]; 
	CreateTexture(TREE_3, pd3dDevice, pd3dCommandList);
	m_TextureStorage[TREE_BG_3] = m_TextureStorage[TREE_3]; 
	CreateTexture(TREE_2, pd3dDevice, pd3dCommandList);
	m_TextureStorage[TREE_BG_2] = m_TextureStorage[TREE_2];


	// 몬스터 들의 텍스쳐 미리 로드
	CreateTexture(CREEPYMONSTER, pd3dDevice, pd3dCommandList);
	CreateTexture(MUSHROOM, pd3dDevice, pd3dCommandList);
	CreateTexture(SPACECAT_GREEN, pd3dDevice, pd3dCommandList);
	CreateTexture(SPACECAT_BLUE, pd3dDevice, pd3dCommandList);
	CreateTexture(SPACECAT_PINK, pd3dDevice, pd3dCommandList);


	// 그외
	CreateTexture("Red", pd3dDevice, pd3dCommandList);

}
 
Texture * const TextureStorage::GetTexture(const std::string & name)
{
	if (m_TextureStorage.find(name) == m_TextureStorage.end()) return nullptr;
	return m_TextureStorage[name];
}
 