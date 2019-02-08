#pragma once
#include "ComponentBase.h"

class Light
	: public ComponentBase
{
protected:
	// 공통적으로 들어감
	XMFLOAT4				m_xmf4Ambient;
	XMFLOAT4				m_xmf4Diffuse;
	XMFLOAT4				m_xmf4Specular;

	bool					m_bEnable;

public:
	Light(GameObject* pOwner);
	virtual ~Light();

	virtual void Update(float fTimeElapsed) override;

};