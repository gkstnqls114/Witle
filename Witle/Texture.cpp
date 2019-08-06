#include "stdafx.h"
#include "TextureStorage.h"
#include "MyDescriptorHeap.h"
#include "d3dUtil.h"

// Scene /////////////////////
#include "MainScene.h"
#include "GameScene.h"
#include "MainScene.h"
#include "SkillSelectScene.h"
// Scene /////////////////////

#include "Texture.h"

Texture::Texture(ENUM_SCENE SceneType, ROOTPARAMETER_INDEX rpIndex, bool bAutoIncrement, int nTextures, UINT nTextureType, int nSamplers)
{ 

	m_nTextureType = nTextureType;
	m_nTextures = nTextures;
	if (m_nTextures > 0)
	{
		m_pRootArgumentInfos = new SRVROOTARGUMENTINFO[m_nTextures];
		m_ppd3dTextureUploadBuffers = new ID3D12Resource*[m_nTextures];
		m_ppd3dTextures = new ID3D12Resource*[m_nTextures];
	}

	// root parameter �̸� ����
	for (int x = 0; x < m_nTextures; ++x)
	{
		m_pRootArgumentInfos[x].m_nRootParameterIndex = (bAutoIncrement) ? (rpIndex.rpIndex + x) : rpIndex.rpIndex;
		m_pRootArgumentInfos[x].m_d3dSrvGpuDescriptorHandle.ptr = NULL;
	}

	m_nSamplers = nSamplers;
	if (m_nSamplers > 0) m_pd3dSamplerGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nSamplers];


	switch (SceneType)
	{
	case ENUM_SCENE::SCENE_NONE:

		break;
	case ENUM_SCENE::SCENE_MAIN:

		break;
	case ENUM_SCENE::SCENE_GAME:
		GameScene::ConnectTexture(this);
		break;

	case ENUM_SCENE::SCENE_SKILLSELECT:

		break;
	default:
		break;
	}
}

Texture::~Texture()
{
	if (m_ppd3dTextures)
	{
		for (int i = 0; i < m_nTextures; i++)
		{
			if (m_ppd3dTextures[i]) m_ppd3dTextures[i]->Release();
		}

		delete[] m_ppd3dTextures;
	}

	if (m_pRootArgumentInfos)
	{
		delete[] m_pRootArgumentInfos;
	}

	if (m_pd3dSamplerGpuDescriptorHandles) delete[] m_pd3dSamplerGpuDescriptorHandles;
}

void Texture::SetRootArgument(int nIndex, UINT nRootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	m_pRootArgumentInfos[nIndex].m_nRootParameterIndex = nRootParameterIndex;
	m_pRootArgumentInfos[nIndex].m_d3dSrvGpuDescriptorHandle = d3dSrvGpuDescriptorHandle;
}

void Texture::SethGPU(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{ 
	m_pRootArgumentInfos[nIndex].m_d3dSrvGpuDescriptorHandle = d3dSrvGpuDescriptorHandle;
}

void Texture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	m_pd3dSamplerGpuDescriptorHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void Texture::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{ 
	if (m_nTextureType == RESOURCE_TEXTURE2D_ARRAY)
	{
		pd3dCommandList->SetGraphicsRootDescriptorTable(m_pRootArgumentInfos[0].m_nRootParameterIndex, m_pRootArgumentInfos[0].m_d3dSrvGpuDescriptorHandle);
	}
	else
	{
		for (int i = 0; i < m_nTextures; i++)
		{
			pd3dCommandList->SetGraphicsRootDescriptorTable(m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);
		}
	}
}

void Texture::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, int nIndex)
{ 
	pd3dCommandList->SetGraphicsRootDescriptorTable(m_pRootArgumentInfos[nIndex].m_nRootParameterIndex, m_pRootArgumentInfos[nIndex].m_d3dSrvGpuDescriptorHandle);
}

void Texture::ReleaseShaderVariables()
{
}

void Texture::ReleaseUploadBuffers()
{
	if (m_ppd3dTextureUploadBuffers)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextureUploadBuffers[i]) m_ppd3dTextureUploadBuffers[i]->Release();
		delete[] m_ppd3dTextureUploadBuffers;
		m_ppd3dTextureUploadBuffers = NULL;
	}  
}

void Texture::ReleaseObjects()
{
	ReleaseShaderVariables();
}

void Texture::ShowPath()
{
#if _DEBUG
	std::wcout << path << std::endl;
#endif // _DEBUG

}

void Texture::LoadTextureFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const wchar_t *pszFileName, UINT nIndex)
{
#if _DEBUG
	path = pszFileName;
#endif // _DEBUG

	m_ppd3dTextures[nIndex] = d3dUtil::CreateTextureResourceFromFile(pd3dDevice, pd3dCommandList, pszFileName, &m_ppd3dTextureUploadBuffers[nIndex], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void Texture::LoadTextureFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const wchar_t * pszFileName, UINT nIndex, bool bIsDDSFile)
{
#if _DEBUG
	path = pszFileName;
#endif // _DEBUG

	if (bIsDDSFile)
		m_ppd3dTextures[nIndex] = d3dUtil::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, &(m_ppd3dTextureUploadBuffers[nIndex]), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	else
		m_ppd3dTextures[nIndex] = d3dUtil::CreateTextureResourceFromWICFile(pd3dDevice, pd3dCommandList, pszFileName, &(m_ppd3dTextureUploadBuffers[nIndex]), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	  
}

