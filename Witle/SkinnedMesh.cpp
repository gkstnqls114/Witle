#include "stdafx.h"
#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh(GameObject * pOwner)
	: ComponentBase(pOwner)
{
	m_FamilyID.InitSkinnedMesh();
}

SkinnedMesh::~SkinnedMesh()
{
}
