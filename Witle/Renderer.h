#pragma once
#include "ComponentBase.h"
class Renderer :
	public ComponentBase
{
public:
	Renderer();
	virtual ~Renderer();
	 
	virtual void Update();
	virtual void Render();

private:

};

