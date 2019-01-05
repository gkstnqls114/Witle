#include "stdafx.h"
#include "CubeMesh.h"
 

CubeMesh::CubeMesh(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, float width, float height, float depth)
{
	m_ComponenetID = MESH_TYPE_ID::CUBE_MESH;
}

CubeMesh::~CubeMesh()
{
}

void CubeMesh::Create()
{
}

void CubeMesh::Init()
{
}
