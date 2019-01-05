#pragma once
#include "Renderer.h"

class Mesh;

class MeshRenderer :
	public Renderer
{ 

public:
	MeshRenderer();
	virtual ~MeshRenderer();
	 
	virtual void Update();
	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, Mesh* mesh);

};

