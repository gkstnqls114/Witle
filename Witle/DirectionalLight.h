#pragma once
#include "Light.h"

class DirectionalLight
	: public Light
{
private:
	XMFLOAT3				m_xmf3Direction; 

public:
	DirectionalLight(GameObject * pOwner);
	virtual ~DirectionalLight();
};