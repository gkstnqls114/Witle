#pragma once
#include "ComponentBase.h"
class Renderer :
	public ComponentBase
{
public:
	Renderer();
	virtual ~Renderer();
	 
	virtual void Update() = 0;
	virtual void Render() = 0;

private:

};

