#pragma once 
#include "Light.h"

class SpotLight
	: public Light
{
private:
	XMFLOAT3				m_xmf3Position;
	float 					m_fFalloff; // ���ʿ����� �ٱ��� ������ ���谡 �Ͼ��.
	XMFLOAT3				m_xmf3Direction;
	float 					m_fTheta; //cos(m_fTheta)
	XMFLOAT3				m_xmf3Attenuation; // ����
	float					m_fPhi; //cos(m_fPhi)
	float					m_fRange; // ������ �ش� �Ÿ����� ������ �ش�.
	float					padding;

public:
	SpotLight(GameObject * pOwner);
	virtual ~SpotLight();
};