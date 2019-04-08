#include "stdafx.h"
#include "Object.h" 
#include "StaticObjectStorage.h"

StaticObjectStorage* StaticObjectStorage::m_Instance{ nullptr };

StaticObjectStorage * StaticObjectStorage::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new StaticObjectStorage();
	}

	return m_Instance;
}

void StaticObjectStorage::Render(ID3D12GraphicsCommandList * pd3dCommandList, int terrainID)
{

}

void StaticObjectStorage::CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, int TerrainPieceCount)
{
	if (m_isCreate) return;

	m_isCreate = true;
}
 