#pragma once
#include "Light.h"

class PointLight
	: public Light
{
private:
	XMFLOAT3				m_xmf3Position;
	XMFLOAT3				m_xmf3Attenuation; // ����
	float					m_fRange; // ������ �ش� �Ÿ����� ������ �ش�.
	XMFLOAT2				padding;

public:
	PointLight(GameObject * pOwner);
	virtual ~PointLight();
};