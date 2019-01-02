#pragma once
#include "Renderer.h"
class MeshRenderer :
	public Renderer
{
	virtual void Update();
	virtual void Render();

public:
	MeshRenderer();
	virtual ~MeshRenderer();
};

