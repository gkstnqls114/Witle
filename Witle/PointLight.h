#pragma once
#include "Light.h"

class PointLight
	: public Light
{
private:
	XMFLOAT3				m_xmf3Position;
	XMFLOAT3				m_xmf3Attenuation; // 감쇠
	float					m_fRange; // 조명은 해당 거리까지 영향을 준다.
	XMFLOAT2				padding;

public:
	PointLight(GameObject * pOwner);
	virtual ~PointLight();
};