#pragma once
#include "ComponentBase.h"

class Material
	: public ComponentBase
{
private:
	XMFLOAT4						m_xmf4Ambient;
	XMFLOAT4						m_xmf4Diffuse;
	XMFLOAT4						m_xmf4Specular; //(r,g,b,a=power)
	XMFLOAT4						m_xmf4Emissive;

public:
	Material(GameObject* pOwner);
	virtual ~Material();

	virtual void Update(float fTimeElapsed);
};