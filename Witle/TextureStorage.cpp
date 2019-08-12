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

void TextureStorage::CreateModelTexture(const std::string & name, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	assert(m_TextureStorage.find(name) == m_TextureStorage.end());
	
	m_TextureStorage[name] = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	std::wstring path = { L"Model/Textures/" };
	std::wstring wname;
	wname.assign(name.begin(), name.end()); 
	path.append(wname);
	path.append(L".dds");
	m_TextureStorage[name]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, path.c_str(), 0);
}


void TextureStorage::CreateImageTexture(const std::string & name, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	assert(m_TextureStorage.find(name) == m_TextureStorage.end());

	m_TextureStorage[name] = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	std::wstring path = { L"Image/" };
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
	// �ͷ��� ������Ʈ ���� �ؽ��� �̸� �ε�
	CreateModelTexture(ALTAR_IN, pd3dDevice, pd3dCommandList);
	CreateModelTexture(ALTAR_OUT, pd3dDevice, pd3dCommandList); 
	CreateModelTexture(BUSH, pd3dDevice, pd3dCommandList);
	m_TextureStorage[BUSHSQUARE] = m_TextureStorage[BUSH]; 
	CreateModelTexture(Cliff, pd3dDevice, pd3dCommandList);
	CreateModelTexture(Flower, pd3dDevice, pd3dCommandList);
	CreateModelTexture(REED, pd3dDevice, pd3dCommandList); 
	CreateModelTexture(RUIN_ARCH, pd3dDevice, pd3dCommandList);
	CreateModelTexture(RUIN_BROKENPILLA, pd3dDevice, pd3dCommandList);
	CreateModelTexture(RUIN_FLOOR, pd3dDevice, pd3dCommandList);
	CreateModelTexture(RUIN_PILLAR, pd3dDevice, pd3dCommandList);
	CreateModelTexture(RUIN_SQUARE, pd3dDevice, pd3dCommandList); 
	CreateModelTexture(SUNFLOWER, pd3dDevice, pd3dCommandList); 
	CreateModelTexture(TREE_1, pd3dDevice, pd3dCommandList);
	m_TextureStorage[TREE_BG_1] = m_TextureStorage[TREE_1]; 
	CreateModelTexture(TREE_3, pd3dDevice, pd3dCommandList);
	m_TextureStorage[TREE_BG_3] = m_TextureStorage[TREE_3]; 
	CreateModelTexture(TREE_2, pd3dDevice, pd3dCommandList);
	m_TextureStorage[TREE_BG_2] = m_TextureStorage[TREE_2];


	// ���� ���� �ؽ��� �̸� �ε�
	CreateModelTexture(CREEPYMONSTER, pd3dDevice, pd3dCommandList);
	CreateModelTexture(MUSHROOM, pd3dDevice, pd3dCommandList);
	CreateModelTexture(SPACECAT_GREEN, pd3dDevice, pd3dCommandList);
	CreateModelTexture(SPACECAT_BLUE, pd3dDevice, pd3dCommandList);
	CreateModelTexture(SPACECAT_PINK, pd3dDevice, pd3dCommandList);
	CreateModelTexture("Shield", pd3dDevice, pd3dCommandList);
	
	// �⺻ �̹�����
	CreateImageTexture(IMAGE_MONSTERATTACKHIT_SPRRITE, pd3dDevice, pd3dCommandList);
	CreateImageTexture(IMAGE_FIREBALLHIT_SPRRITE, pd3dDevice, pd3dCommandList);
	CreateImageTexture(IMAGE_ICEBALLHIT_SPRRITE, pd3dDevice, pd3dCommandList);
	CreateImageTexture(IMAGE_LIGHTNINGBALLHIT_SPRRITE, pd3dDevice, pd3dCommandList);
	CreateImageTexture(IMAGE_NORMAL_SPRRITE, pd3dDevice, pd3dCommandList);
	CreateImageTexture(IMAGE_RED, pd3dDevice, pd3dCommandList);
	CreateImageTexture("Blue", pd3dDevice, pd3dCommandList);
	CreateImageTexture("Yellow", pd3dDevice, pd3dCommandList);
	CreateImageTexture("Lose", pd3dDevice, pd3dCommandList);
	CreateImageTexture("Win", pd3dDevice, pd3dCommandList);
	CreateImageTexture("Wittle_1280x720", pd3dDevice, pd3dCommandList);

}
 
Texture * const TextureStorage::GetTexture(const std::string & name)
{
	if (m_TextureStorage.find(name) == m_TextureStorage.end())
	{
		return nullptr;
	}
	return m_TextureStorage[name];
}
 