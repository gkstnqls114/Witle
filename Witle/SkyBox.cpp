#include "stdafx.h"
#include "ShaderManager.h"
#include "Object.h"
#include "MyDescriptorHeap.h"
#include "CMesh.h"
#include "Texture.h"
#include "SkyBox.h"

void SkyBox::ReleaseMembers()
{
	if (m_Heap)
	{
		m_Heap->ReleaseObjects();
		delete m_Heap;
		m_Heap = nullptr;
	}
	if (m_Texture)
	{
		m_Texture->ReleaseShaderVariables();
		delete m_Texture;
		m_Texture = nullptr;
	}
	if (m_LoadObject)
	{
		m_LoadObject->ReleaseObjects();
		delete m_LoadObject;
		m_LoadObject = nullptr;
	}
}

void SkyBox::ReleaseMemberUploadBuffers()
{
	if (m_Texture) m_Texture->ReleaseUploadBuffers();
	if(m_LoadObject) m_LoadObject->ReleaseUploadBuffers();
}

SkyBox::SkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth, float fHeight, float fDepth)
	:GameObject("SkyBox")
{ 
	m_LoadObject = new LoadObject(1);

	CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(pd3dDevice, pd3dCommandList, fWidth, fHeight, fDepth);
	m_LoadObject->SetMesh(pSkyBoxMesh);
	m_LoadObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Texture *pSkyBoxTexture = new Texture(1, RESOURCE_TEXTURE_CUBE, 0);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/SkyBox_0.dds", 0);
	m_Texture = pSkyBoxTexture;
	 
	m_Heap = new MyDescriptorHeap();
	m_Heap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
	m_Heap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pSkyBoxTexture, ROOTPARAMETER_SKYBOX, RESOURCE_TEXTURE_CUBE, 0);

	CMaterial *pSkyBoxMaterial = new CMaterial(1);
	// pSkyBoxMaterial->SetTexture(pSkyBoxTexture);
	// pSkyBoxMaterial->SetShader(ShaderManager::GetInstance()->SetShader(pd3dCommandList, "SkyBoxShader"));
	 
	m_LoadObject->SetMaterial(0, pSkyBoxMaterial);
}

SkyBox::~SkyBox()
{

}
void SkyBox::Update(float fElapsedTime)
{ 
	m_Transform.SetPosition(m_pPlayerTransform->GetPosition());
	m_Transform.Update(fElapsedTime);
	m_LoadObject->UpdateTransform(m_Transform.GetpWorldMatrix());
}

void SkyBox::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "SkyBoxShader", isGBuffers);
	m_Heap->UpdateShaderVariable(pd3dCommandList);
	m_Texture->UpdateShaderVariables(pd3dCommandList);
	m_LoadObject->Render(pd3dCommandList);
}
