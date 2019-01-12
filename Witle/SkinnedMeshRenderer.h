#pragma once
#include "Renderer.h"


class SkinnedMeshRenderer :
	public Renderer
{
private:
	
public:
	SkinnedMeshRenderer();
	virtual ~SkinnedMeshRenderer();

	virtual void Update();
	virtual void Render(ID3D12GraphicsCommandList * pCommandList, Mesh* mesh);

};

