#pragma once
#include "ComponentBase.h"

class MyFrustum
	: ComponentBase
{
	BoundingFrustum m_frustum;

public:
	MyFrustum(GameObject* pOwner);
	MyFrustum(GameObject* pOwner, const XMFLOAT3& origin, const XMFLOAT4& orientation, float rightSlope, float leftSlope, float topSlope, float bottomSlope, float frustum_near, float frustum_far)
		: m_frustum{ origin, orientation, rightSlope, leftSlope, topSlope, bottomSlope, frustum_near, frustum_far } ,
		  ComponentBase(pOwner)
	{};
	~MyFrustum();

	virtual void Update(float fTimeElapsed) {};

	bool IsContain(const XMFLOAT3& pos);
	bool IsContain(const BoundingBox& box);
};