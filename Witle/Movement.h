#pragma once
#include "ComponentBase.h"

// ���� ������Ʈ �̵��� �ʿ��� ���ӵ�, ȸ������ ���� �������� ���ϰ� �ֽ��ϴ�.
// �̵��� �ʿ��� ���ӵ�, �߷°��ӵ�, �ִ� ���ӵ�, ������ ���� �����մϴ�.
class Movement
	: public ComponentBase
{
protected:
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f); // ���ӵ�

public:  
	XMFLOAT3 m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);  // �߷°��ӵ�
	float m_fMaxVelocityY{ 400.0f };					  // �ִ� y�� ���ӵ�
	float m_fMaxVelocityXZ{ 2000.0f };					  // �ִ� x��� z�� ���ӵ�
	float m_fFriction = 1700.0f;						  // ������
	float m_fDistance = 2000.f;							  // �ʴ� m (2000 = 20m)

	float m_fPitch = 0.0f;								  // y�� ȸ��
	float m_fRoll = 0.0f;								  // x�� ȸ��
	float m_fYaw = 0.0f;							      // z�� ȸ��

protected:
	// shift ��ŭ ���ӵ��� �����մϴ�.
	void MoveVelocity(const XMFLOAT3& shift);

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	Movement(GameObject* pOwner);
	virtual ~Movement();

	virtual void Update(float) override;

	// �ش� �������� ���ӵ��� ����� ���� �����մϴ�.
	XMFLOAT3 AlreadyUpdate(float);

	const XMFLOAT3& GetVelocity() const { return m_xmf3Velocity; }
	void SetVelocity(const XMFLOAT3& velocity) { m_xmf3Velocity = velocity; }
	void SetVelocity(XMFLOAT3&& velocity) { m_xmf3Velocity = velocity; }
};