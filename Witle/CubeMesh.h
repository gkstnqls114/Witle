#pragma once
#include "Mesh.h"
class CubeMesh :
	public Mesh
{
public:
	CubeMesh();
	virtual ~CubeMesh();
	 
	virtual void Create();
	virtual void Init();
};

