#include "stdafx.h"
#include "QuadtreeNode.h"
 
quadtree::BASE_NODE::BASE_NODE(XMFLOAT3&& center, XMFLOAT3&& extents)
{
	BoBox = new MyBOBox(center, extents); 
}


quadtree::BASE_NODE::BASE_NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
{
	BoBox = new MyBOBox(center, extents);
}

quadtree::BASE_NODE::~BASE_NODE()
{
	if (BoBox)
	{
		delete BoBox;
		BoBox = nullptr;
	}

}

quadtree::COLLISION_NODE::COLLISION_NODE(XMFLOAT3&& center, XMFLOAT3&& extents)
 : quadtree::BASE_NODE::BASE_NODE(center, extents)
{
}

quadtree::COLLISION_NODE::COLLISION_NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
	: quadtree::BASE_NODE::BASE_NODE( center, extents)
{
}

quadtree::COLLISION_NODE::~COLLISION_NODE()
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

quadtree::COLLIDER::COLLIDER(COLLIDER const& other)
	: BoBox(other.BoBox), World(other.World) 
{

}
