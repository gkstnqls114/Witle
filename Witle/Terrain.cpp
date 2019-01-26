#include "stdafx.h"
#include "TerrainMesh.h"
#include "Texture.h"
#include "HeightMapImage.h"
#include "Terrain.h"

Terrain::Terrain(const std::string& entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, LPCTSTR  pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
	:GameObject(entityID)
{
	if (pFileName == nullptr)
	{
#ifdef _DEBUG
		std::cout << "file name is nullptr";
#endif
		return;
	}

	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	m_xmf3Scale = xmf3Scale;

	m_pHeightMapImage = new HeightMapImage(pFileName, nWidth, nLength, xmf3Scale);

	// m_nMeshes = cxBlocks * czBlocks;
	// m_ppMeshes = new CMesh*[m_nMeshes];
	// for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = NULL;

	//CHeightMapGridMesh *pHeightMapGridMesh = NULL;
	//for (int z = 0, zStart = 0; z < czBlocks; z++)
	//{
	//	for (int x = 0, xStart = 0; x < cxBlocks; x++)
	//	{
	//		xStart = x * (nBlockWidth - 1);
	//		zStart = z * (nBlockLength - 1);
	//		pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
	//		SetMesh(x + (z*cxBlocks), pHeightMapGridMesh);
	//	}
	//}

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	TerrainMesh *pterrainMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			pterrainMesh = new TerrainMesh(this, pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			// SetMesh(x + (z*cxBlocks), pHeightMapGridMesh);
		}
	}

	InsertComponent("TerrainMesh", pterrainMesh);

	Texture *pTerrainTexture = new Texture(2, RESOURCE_TEXTURE2D, 0);

	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Base_Texture.dds", 0);
	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Detail_Texture_7.dds", 1);

	m_ResourceBase = pTerrainTexture;
	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수

	//CTerrainShader *pTerrainShader = new CTerrainShader();
	//pTerrainShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	//pTerrainShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//pTerrainShader->CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 2);
	//pTerrainShader->CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 1, m_pd3dcbGameObject, ncbElementBytes);
	//pTerrainShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTerrainTexture, 4, true);

	//CMaterial *pTerrainMaterial = new CMaterial();
	//pTerrainMaterial->SetTexture(pTerrainTexture);

	//SetMaterial(pTerrainMaterial);

	//SetCbvGPUDescriptorHandle(pTerrainShader->GetGPUCbvDescriptorStartHandle());

	//SetShader(pTerrainShader);
}

Terrain::~Terrain()
{
	ReleaseMembers();
}

void Terrain::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 힙 설정
	// pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	static_cast<Texture *>(m_ResourceBase)->UpdateShaderVariables(pd3dCommandList);
}

void Terrain::Create()
{
}

void Terrain::Init()
{
}

void Terrain::ReleaseMembers()
{
	if (m_pHeightMapImage)
	{
		delete m_pHeightMapImage;
		m_pHeightMapImage = nullptr;
	}
}

float Terrain::GetHeight(float x, float z, bool bReverseQuad)
{
	return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y); // World
}

XMFLOAT3 Terrain::GetNormal(float x, float z)
{
	return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); 
}

int Terrain::GetHeightMapWidth()
{
	return(m_pHeightMapImage->GetHeightMapWidth());
}

int Terrain::GetHeightMapLength()
{
	return(m_pHeightMapImage->GetHeightMapLength());
}
