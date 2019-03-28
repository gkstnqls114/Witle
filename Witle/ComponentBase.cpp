#include "stdafx.h"
#include "ComponentBase.h"


ComponentBase::~ComponentBase()
{
	// assert(!(m_pOwner == nullptr)); // 만약 게임오브젝트 오너가 설정되지 않는다면 잘못되었다.
}

ResourceComponentBase::~ResourceComponentBase()
{
}
