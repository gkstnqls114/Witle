#pragma once
#include "ComponentBase.h"

class MyRigidbody
	: public ComponentBase
{
	float m_Mass{ 1.f }; //������Ʈ�� ����
	float m_Drag{ 0.0f }; // ��������, 0�Ͻ� ����


public:
	MyRigidbody();
	virtual ~MyRigidbody();

	virtual void Update() override;

	void AddForce(const XMFLOAT3& force);
	void AddForce(float x, float y, float z);
};