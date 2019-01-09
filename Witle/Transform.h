#pragma once
#include "ComponentBase.h"
class Transform :
	public ComponentBase
{
	XMFLOAT3 m_Right{1 , 0, 0};
	XMFLOAT3 m_Up{ 0, 1, 0 };
	XMFLOAT3 m_Look{ 0, 0, 1 };
	XMFLOAT3 m_Position{ 0, 0, 0 };

	XMFLOAT4X4 m_transformWorld { Matrix4x4::Identity() };

public:
	Transform();
	virtual ~Transform();

	virtual void Update() override;

	void Rotate(float x, float y, float z);

	
	//////////////////////////////////////////////// Get
	XMFLOAT4X4 GetWorldMatrix() const { return m_transformWorld; }
	XMFLOAT3 GetRight() const { return m_Right; }
	XMFLOAT3 Getup() const { return m_Up; }
	XMFLOAT3 GetLook() const { return m_Look; }
	XMFLOAT3 GetPosition() const { return m_Position; }
	//////////////////////////////////////////////// Get

};

