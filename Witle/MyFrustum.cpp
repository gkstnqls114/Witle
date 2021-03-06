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
	//원근 투영 변환 행렬에서 절두체를 생성한다(절두체는 카메라 좌표계로 표현된다).
	m_frustum.CreateFromMatrix(m_frustum, XMLoadFloat4x4(&projection));
	
	// 카메라 변환 행렬의 역행렬을 구한다. 
	//절두체를 카메라 변환 행렬의 역행렬로 변환한다(이제 절두체는 월드 좌표계로 표현된다).
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
 