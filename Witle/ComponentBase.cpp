#include "stdafx.h"
#include "ComponentBase.h"


ComponentBase::~ComponentBase()
{
	// assert(!(m_pOwner == nullptr)); // ���� ���ӿ�����Ʈ ���ʰ� �������� �ʴ´ٸ� �߸��Ǿ���.
}

ResourceComponentBase::~ResourceComponentBase()
{
}
