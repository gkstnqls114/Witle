#pragma once 
#include "Light.h"

class SpotLight
	: public Light
{
private:
	XMFLOAT3				m_xmf3Position;
	float 					m_fFalloff; // 안쪽원에서 바깥쪽 원으로 감쇠가 일어난다.
	XMFLOAT3				m_xmf3Direction;
	float 					m_fTheta; //cos(m_fTheta)
	XMFLOAT3				m_xmf3Attenuation; // 감쇠
	float					m_fPhi; //cos(m_fPhi)
	float					m_fRange; // 조명은 해당 거리까지 영향을 준다.
	float					padding;

public:
	SpotLight(GameObject * pOwner);
	virtual ~SpotLight();
};