#pragma once
#include "ComponentBase.h"

class SkinnedMesh
	: public ComponentBase
{

public:
	SkinnedMesh(GameObject* pOwner);
	virtual ~SkinnedMesh();

	virtual void Update(float fTimeElapsed) {};
};