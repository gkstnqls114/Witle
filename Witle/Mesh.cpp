#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(GameObject* pOwner)
	:ComponentBase(pOwner)
{
	m_FamillyID = "Mesh"; 
}


Mesh::~Mesh()
{
}
