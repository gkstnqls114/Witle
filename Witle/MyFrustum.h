#pragma once 
#include "ComponentBase.h"

class QtTerrainInstancingDrawer;  
namespace quadtree
{
	struct QT_DRAWER_NODE;
}

class MyFrustum
	: ComponentBase
{
	BoundingFrustum m_frustum;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MyFrustum(GameObject* pOwner);
	MyFrustum(GameObject* pOwner, const XMFLOAT3& origin, const XMFLOAT4& orientation, float rightSlope, float leftSlope, float topSlope, float bottomSlope, float frustum_near, float frustum_far)
		: m_frustum{ origin, orientation, rightSlope, leftSlope, topSlope, bottomSlope, frustum_near, frustum_far } ,
		  ComponentBase(pOwner) {};

	MyFrustum(GameObject* pOwner, const XMFLOAT4X4& projection, const XMFLOAT4X4 & view);
	
	~MyFrustum();

	virtual void Update(float fTimeElapsed) override {};
	void GenerateFrustum(const XMFLOAT4X4& projection, const XMFLOAT4X4 & view);

	bool IsContain(const XMFLOAT3& pos) const;
	bool IsContain(const BoundingBox& box) const;
	
	bool IsIntersect(const XMFLOAT3& pos) const;
	bool IsIntersect(const BoundingBox& pos) const;

	void CheckRendering(quadtree::QT_DRAWER_NODE* node);
	void CheckRenderingAllTRUE(quadtree::QT_DRAWER_NODE* node);
	void CheckRenderingAllFALSE(quadtree::QT_DRAWER_NODE* node);
	BoundingFrustum GetBoundingFrustum() const { return m_frustum; }
};