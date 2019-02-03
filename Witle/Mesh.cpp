#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(GameObject* pOwner)
	:ComponentBase(pOwner)
{
	m_FamillyID = FAMILLYID_MESH;
}


Mesh::~Mesh()
{
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
