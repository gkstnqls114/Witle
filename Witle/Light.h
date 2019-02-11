#pragma once
#include "ComponentBase.h"

struct LIGHT
{
	XMFLOAT4				Ambient;
	XMFLOAT4				Diffuse;
	XMFLOAT4				Specular;
	XMFLOAT3				Position;
	float 					fFalloff;
	XMFLOAT3				Direction;
	float 					fTheta; //cos(m_fTheta)
	XMFLOAT3				Attenuation;
	float					fPhi; //cos(m_fPhi)
	bool					bEnable;
	int						nType;
	float					fRange;
	float					padding;
};

class Light
	: public ResourceComponentBase
{

protected:
	LIGHT m_Light;

public:
	Light(GameObject* pOwner);
	virtual ~Light();

	virtual void Update(float fTimeElapsed) = 0;

};