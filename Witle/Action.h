#pragma once
#include "ComponentBase.h"
 
class Movement;

// 액션의 부모 클래스.
class Action
	: public ComponentBase
{
public:
	const ENUM_ANIMATIONID m_AnimationID{ ANIMATIONID_ERROR };

public:
	// 초기화 함수.
	virtual void Init() = 0;

	// 가속도를 설정.
	// Scene에서 Update 수행 이전 반드시 호출해야합니다.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	explicit Action(GameObject* pOwner, ENUM_ANIMATIONID animeID) :
		ComponentBase(pOwner), m_AnimationID(animeID) {};
	virtual ~Action() {};
};
