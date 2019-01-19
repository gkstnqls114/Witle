#pragma once
#include "ComponentBase.h"

class MyRigidbody
	: public ComponentBase
{
public:
	MyRigidbody();
	virtual ~MyRigidbody();

	virtual void Update() override;
};