#include "stdafx.h" 
#include "QtTerrainInstancingDrawer.h"
#include "MyFrustum.h"

MyFrustum::MyFrustum(GameObject * pOwner)
	: ComponentBase(pOwner)
{
	m_FamilyID.InitFrustum();
}

MyFrustum::MyFrustum(GameObject * pOwner, const XMFLOAT4X4 & projection, const XMFLOAT4X4 & view)
	: ComponentBase(pOwner)
{
	GenerateFrustum(projection, view);
}

MyFrustum::~MyFrustum()
{
}

void MyFrustum::GenerateFrustum(const XMFLOAT4X4 & projection, const XMFLOAT4X4 & view)
{
	//���� ���� ��ȯ ��Ŀ��� ����ü�� �����Ѵ�(����ü�� ī�޶� ��ǥ��� ǥ���ȴ�).
	m_frustum.CreateFromMatrix(m_frustum, XMLoadFloat4x4(&projection));
	
	// ī�޶� ��ȯ ����� ������� ���Ѵ�. 
	//����ü�� ī�޶� ��ȯ ����� ����ķ� ��ȯ�Ѵ�(���� ����ü�� ���� ��ǥ��� ǥ���ȴ�).
	m_frustum.Transform(m_frustum, XMMatrixInverse(NULL, XMLoadFloat4x4(&view)));
	
}

bool MyFrustum::IsContain(const XMFLOAT3 & pos) const 
{
	return m_frustum.Contains(XMLoadFloat3(&pos));
}

bool MyFrustum::IsContain(const BoundingBox & box) const
{
	return m_frustum.Contains(box);
}

bool MyFrustum::IsIntersect(const XMFLOAT3 & pos) const
{
	return m_frustum.Intersects(XMLoadFloat3(&pos));
}

bool MyFrustum::IsIntersect(const BoundingBox & box) const
{
	return m_frustum.Intersects(box);
}
 