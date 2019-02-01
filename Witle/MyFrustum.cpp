#include "stdafx.h"
#include "MyFrustum.h"

MyFrustum::MyFrustum(GameObject * pOwner)
	: ComponentBase(pOwner)
{
}

MyFrustum::~MyFrustum()
{
}

bool MyFrustum::IsContain(const XMFLOAT3 & pos)
{
	return m_frustum.Contains(XMLoadFloat3(&pos));
}

bool MyFrustum::IsContain(const BoundingBox & box)
{
	return m_frustum.Contains(box);
}
