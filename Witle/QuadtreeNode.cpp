#include "stdafx.h"
#include "QuadtreeNode.h"
 
quadtree::NODE::NODE(XMFLOAT3&& center, XMFLOAT3&& extents)
{
	quadtree::NODE(center, extents);
}


quadtree::NODE::NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
{
	BoBox = new MyBOBox(center, extents);
}

quadtree::NODE::~NODE()
{
	if (BoBox)
	{
		delete BoBox;
		BoBox = nullptr;
	}

	for (int x = 0; x < 4; ++x)
	{
		if (children[x])
		{
			delete children[x];
			children[x] = nullptr;
		}
	}
}
