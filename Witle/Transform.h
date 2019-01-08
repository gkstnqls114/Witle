#pragma once
#include "ComponentBase.h"
class Transform :
	public ComponentBase
{
	XMFLOAT4X4 m_WorldMatrix;

public:
	Transform();
	virtual ~Transform();

	virtual void Update() override;

	XMFLOAT4X4 GetWorldMatrix() const { m_WorldMatrix; }

};

