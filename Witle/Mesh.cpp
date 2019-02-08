#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(GameObject* pOwner)
	:ComponentBase(pOwner)
{
	m_FamilyID = FAMILYID_MESH;
}


Mesh::~Mesh()
{
	if (m_pxmf3Positions)
	{
		delete m_pxmf3Positions;
		m_pxmf3Positions = nullptr;
	}
	if (m_pPositionBuffer)
	{
		m_pPositionBuffer->Release();
		m_pPositionBuffer = nullptr;
	}
	if (m_pVertexBufferViews)
	{
		delete m_pVertexBufferViews;
		m_pVertexBufferViews = nullptr;
	}
	if (m_pnIndices)
	{
		delete m_pnIndices;
		m_pnIndices = nullptr;
	}
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}
}

void Mesh::ReleaseUploadBuffers()
{
	if (m_pPositionUploadBuffer)
	{
		m_pPositionUploadBuffer->Release();
		m_pPositionUploadBuffer = nullptr;
	}
	if (m_pIndexUploadBuffer)
	{
		m_pIndexUploadBuffer->Release();
		m_pIndexUploadBuffer = nullptr;
	}
}
