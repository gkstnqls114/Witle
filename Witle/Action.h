#pragma once
#include "ComponentBase.h"
 
class Movement;

// �׼��� �θ� Ŭ����.
class Action
	: public ComponentBase
{
public:
	const ENUM_ANIMATIONID m_AnimationID{ ANIMATIONID_ERROR };

public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	explicit Action(GameObject* pOwner, ENUM_ANIMATIONID animeID) :
		ComponentBase(pOwner), m_AnimationID(animeID) {};
	virtual ~Action() {};
};
 