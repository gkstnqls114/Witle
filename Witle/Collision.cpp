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

bool Collision::ProcessCollision(const BoundingOrientedBox & moveObject, const MyBOBox & staticObject,
	const XMFLOAT3 & beforUpdatePosition,
	const XMFLOAT3 & nowVelocity,
	float fElapsedTime,
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
			float d = staticObject.GetPlane(x).w;
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
