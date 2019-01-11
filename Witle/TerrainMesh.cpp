#include "stdafx.h"
#include "HeightMapImage.h"
#include "TerrainMesh.h"

TerrainMesh::TerrainMesh(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
	m_ComponenetID = TERRAIN_MESH;
}

TerrainMesh::~TerrainMesh()
{
}

void TerrainMesh::Create()
{
}

void TerrainMesh::Init()
{
}

float TerrainMesh::GetHeight(float x, float z, bool bReverseQuad)
{
	return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y); // World
}

XMFLOAT3 TerrainMesh::GetNormal(float x, float z)
{
	return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); 
}

int TerrainMesh::GetHeightMapWidth()
{
	return(m_pHeightMapImage->GetHeightMapWidth());
}

int TerrainMesh::GetHeightMapLength()
{
	return(m_pHeightMapImage->GetHeightMapLength());
}

XMFLOAT3 GetNormal(float x, float z)
{
	return XMFLOAT3();
}
