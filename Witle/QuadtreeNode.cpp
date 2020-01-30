#include "stdafx.h"
#include "Object.h"
#include "QuadtreeNode.h"
 
quadtree::QT_BASE_NODE::QT_BASE_NODE(XMFLOAT3&& center, XMFLOAT3&& extents)
{
	BoBox = new MyBOBox(center, extents); 
}


quadtree::QT_BASE_NODE::QT_BASE_NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
{
	BoBox = new MyBOBox(center, extents);
}

quadtree::QT_BASE_NODE::~QT_BASE_NODE()
{
	if (BoBox)
	{
		delete BoBox;
		BoBox = nullptr;
	}

}

quadtree::QT_COLLISION_NODE::QT_COLLISION_NODE(XMFLOAT3&& center, XMFLOAT3&& extents)
 : quadtree::QT_BASE_NODE::QT_BASE_NODE(center, extents)
{
}

quadtree::QT_COLLISION_NODE::QT_COLLISION_NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
	: quadtree::QT_BASE_NODE::QT_BASE_NODE( center, extents)
{
}

quadtree::QT_COLLISION_NODE::~QT_COLLISION_NODE()
{
	terrainObjBoBoxs.clear();

	for (int x = 0; x < 4; ++x)
	{
		if (children[x])
		{
			delete children[x];
			children[x] = nullptr;
		}
	}
}

quadtree::QT_COLLIDER_ADDER::QT_COLLIDER_ADDER(QT_COLLIDER_ADDER const& other)
	: BoBox(other.BoBox), World(other.World) 
{

}
 
quadtree::QT_DRAWER_NODE::QT_DRAWER_NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
	: quadtree::QT_BASE_NODE::QT_BASE_NODE(center, extents)
{
}

void quadtree::QT_DRAWER_INFO::ReleaseObject()
{
	TransformList.clear(); 
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Release();
		m_pd3dcbGameObjects = nullptr;
	} 
	if (pLoadObject)
	{
		delete pLoadObject;
		pLoadObject = nullptr;
	}
}
