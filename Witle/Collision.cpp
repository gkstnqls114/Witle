#include "stdafx.h"
#include "MyCollider.h"
#include "MyBOBox.h"
#include "MyBSphere.h"
#include "Collision.h"

bool Collision::isCollide(MyCollider* collider, const XMFLOAT3 & origin, const XMFLOAT3 & direction, float & dist)
{
	switch (collider->GetType())
	{
	case COLLIDER_TYPE::BOUNDING_BOX:
		return isCollide(static_cast<MyBOBox *>(collider)->GetBOBox(), origin, direction, dist);
		break;

	default:
		// ����� ���� �����Ƿ� ���â�� ����ϴ�.
		assert(false);
		return false;
		break;
	}
}

bool Collision::ProcessCollision(const BoundingOrientedBox & moveObject,
	const MyBOBox & staticObject, 
	const XMFLOAT3 & beforUpdatePosition,
	const XMFLOAT3 & nowVelocity,
	float fElapsedTime,
	bool StaticObjIsMove,
	XMFLOAT3 & SlideVector)
{
	// �̵��� �ڽ��� ���� �浹�Ѵ�.
	bool isAlreadyCollide = Collision::isCollide(moveObject, staticObject.GetBOBox());
	if (isAlreadyCollide)
	{
		for (int x = 0; x < 4; ++x) //  plane 4 �鿡 ���� üũ�Ѵ�..
		{
			XMFLOAT3 intersectionPoint;
			// ���⼭ d��... ������ ������ �Ÿ��� �ǹ��Ѵ�. (���/����)
			float d = 0;
			if (StaticObjIsMove)
			{
				d = Plane::CaculateD(staticObject.GetPlane(x), staticObject.GetPosOnPlane(x));		
			}
			else
			{
				d = staticObject.GetPlane(x).w;
			}
			
			// �ð��� ���� �̵��� �������� �ϹǷ� velocity �� ������ �ð��� ���Ѵ�.
			bool isFront = Plane::IsFront(staticObject.GetPlaneNormal(x), d, beforUpdatePosition); // ������Ʈ ���� ��ġ
			// ���� ������ ��鿡 �����ߴٸ�...
			if (isFront)
			{
				bool isIntersect = Plane::Intersect(staticObject.GetPlaneNormal(x),
					d, beforUpdatePosition, 
					Vector3::ScalarProduct(nowVelocity, fElapsedTime, false),
					intersectionPoint);

				if (isIntersect)
				{
					// �ش� �������� �������ȿ� �����ϴ��� Ȯ���Ѵ�.
					// staticObject[i].IsIn(x, intersectionPoint)
					// 
					if (Collision::isIn(staticObject.GetBOBox(), intersectionPoint)) //isIn �Լ� ���߿� �����ؾ���
					{
						SlideVector =
							Vector3::Sliding(staticObject.GetPlaneNormal(x), nowVelocity); 
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Collision::isCollide(MyCollider * a, MyCollider * b)
{
	COLLIDER_TYPE A_type = a->GetType();
	COLLIDER_TYPE B_type = b->GetType();

	if (A_type == COLLIDER_TYPE::BOUNDING_BOX && B_type == COLLIDER_TYPE::BOUNDING_BOX)
	{
		BoundingOrientedBox collider1 = static_cast<MyBOBox*>(a)->GetBOBox();
		BoundingOrientedBox collider2 = static_cast<MyBOBox*>(b)->GetBOBox();
		return collider1.Intersects(collider2);
	}

	if (A_type == COLLIDER_TYPE::BOUNDING_BOX && B_type == COLLIDER_TYPE::BOUNDING_SPHERE)
	{  
		BoundingOrientedBox collider1 = static_cast<MyBOBox*>(a)->GetBOBox();
		BoundingSphere* collider2 = static_cast<MyBSphere*>(b)->GetBSphere();
		return collider1.Intersects(*collider2);
	}
	if (A_type == COLLIDER_TYPE::BOUNDING_SPHERE && B_type == COLLIDER_TYPE::BOUNDING_BOX)
	{
		BoundingSphere* collider1 = static_cast<MyBSphere*>(a)->GetBSphere();
		BoundingOrientedBox collider2 = static_cast<MyBOBox*>(b)->GetBOBox();
		return collider1->Intersects(collider2);
	}
	if (A_type == COLLIDER_TYPE::BOUNDING_SPHERE && B_type == COLLIDER_TYPE::BOUNDING_SPHERE)
	{ 
		BoundingSphere* collider1 = static_cast<MyBSphere*>(a)->GetBSphere();
		BoundingSphere* collider2 = static_cast<MyBSphere*>(b)->GetBSphere();
		return collider1->Intersects(*collider2);
	} 

	// �ش� ����� ���� �����Ƿ� ���â�� ����ϴ�.
	assert(false);
}
