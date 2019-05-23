#include "stdafx.h"
#include "MyCollider.h"
#include "MyBOBox.h"
#include "Collision.h"

bool Collision::isCollide(MyCollider* collider, const XMFLOAT3 & origin, const XMFLOAT3 & direction, float & dist)
{
	switch (collider->GetType())
	{
	case COLLIDER_TYPE::BOUNDING_BOX:
		return isCollide(static_cast<MyBOBox *>(collider)->GetBOBox(), origin, direction, dist);
		break;

	default:
		return false;
		break;
	}
}
