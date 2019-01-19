#pragma once
#include "ComponentBase.h"

class MyRigidbody
	: public ComponentBase
{
	float m_Mass{ 1.f }; //오브젝트의 질량
	float m_Drag{ 0.0f }; // 공기저항, 0일시 없음


public:
	MyRigidbody();
	virtual ~MyRigidbody();

	virtual void Update() override;

	void AddForce(const XMFLOAT3& force);
	void AddForce(float x, float y, float z);
};