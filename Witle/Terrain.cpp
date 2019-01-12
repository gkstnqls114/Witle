#include "stdafx.h"
#include "HeightMapImage.h"
#include "Terrain.h"

Terrain::Terrain(const std::string& entityID, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
	:GameObject(entityID)
{
}

Terrain::~Terrain()
{
}

void Terrain::Create()
{
}

void Terrain::Init()
{
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

XMFLOAT3 GetNormal(float x, float z)
{
	return XMFLOAT3();
}
