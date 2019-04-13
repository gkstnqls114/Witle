#include "stdafx.h"
#include "Object.h"
#include "CMesh.h"
#include "Texture.h"
#include "SkyBox.h"

SkyBox::SkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth, float fHeight, float fDepth)
	:GameObject("SkyBox")
{ 
	m_LoadObject = new LoadObject();

	CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 2.0f);
	m_LoadObject->SetMesh(pSkyBoxMesh);
	m_LoadObject->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	Texture *pSkyBoxTexture = new Texture(1, RESOURCE_TEXTURE_CUBE, 0);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"SkyBox/SkyBox_0.dds", 0);

	//CSkyBoxShader *pSkyBoxShader = new CSkyBoxShader();
	//pSkyBoxShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	//pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	//CScene::CreateShaderResourceViews(pd3dDevice, pSkyBoxTexture, 10, false);

	//CMaterial *pSkyBoxMaterial = new CMaterial(1);
	//pSkyBoxMaterial->SetTexture(pSkyBoxTexture);
	//pSkyBoxMaterial->SetShader(pSkyBoxShader);

	//SetMaterial(0, pSkyBoxMaterial);
}

SkyBox::~SkyBox()
{

}
