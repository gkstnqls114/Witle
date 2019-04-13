#pragma once
#include "ComponentBase.h"

struct AXIS
{
	XMFLOAT3 right{ 1 , 0, 0 };
	XMFLOAT3 up{ 0, 1, 0 };
	XMFLOAT3 look{ 0, 0, 1 };

	AXIS(const XMFLOAT3& iright, const XMFLOAT3& iup, const XMFLOAT3& ilook) : right(iright), up(iup), look(ilook) {};
};

class Transform :
	public ComponentBase
{
	XMFLOAT3 m_Right{1 , 0, 0};
	XMFLOAT3 m_Up{ 0, 1, 0 };
	XMFLOAT3 m_Look{ 0, 0, 1 };
	XMFLOAT3 m_Position{ 0, 0, 0 };

	XMFLOAT4X4 m_transformWorld { Matrix4x4::Identity() };

	void SetTransform(const XMFLOAT3& right, const XMFLOAT3& up, const XMFLOAT3& look, const XMFLOAT3& pos);
public:
	Transform(GameObject* pOwner);
	virtual ~Transform();

	virtual void Update(float fTimeElapsed) override;

	void Rotate(float x, float y, float z);
	
	//////////////////////////////////////////////// Get
	void SetPosition(const XMFLOAT3& pos) { m_Position = pos; }
	void SetPosition(float x, float y, float z) { m_Position = XMFLOAT3(x, y, z); }
	void SetRight(const XMFLOAT3& right) { m_Right = right; }
	void SetUp(const XMFLOAT3& up) { m_Up = up; }
	void SetLook(const XMFLOAT3& look) { m_Look = look; }
	//////////////////////////////////////////////// Get

	//////////////////////////////////////////////// Get
	const XMFLOAT4X4& GetWorldMatrix() const { return m_transformWorld; }
	XMFLOAT4X4* GetpWorldMatrix()  { return &m_transformWorld; }
	const XMFLOAT3& GetRight() const { return m_Right; }
	const XMFLOAT3& GetUp() const { return m_Up; }
	const XMFLOAT3& GetLook() const { return m_Look; }
	const XMFLOAT3& GetPosition() const { return m_Position; }
	const AXIS GetCoorAxis() const { return AXIS(m_Right, m_Up, m_Look); }
	//////////////////////////////////////////////// Get

	void Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity); 

	void Move(const XMFLOAT3& vMove); // vMove만큼 Position 에서 움직인다.
	
	Transform& operator=(const Transform& pTr)
	{
		SetTransform(pTr.m_Right, pTr.m_Up, pTr.m_Look, pTr.m_Position);
		return *this;
	}
};

