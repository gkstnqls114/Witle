#include "stdafx.h"
#include "MyCollider.h"
#include "MyBOBox.h"
#include "MyBBox.h"
#include "MyBSphere.h"
#include "Player.h"
#include "Monster.h"
#include "ModelStorage.h" 
#include "QtTerrainInstancingDrawer.h"
#include "GameTimer.h"
#include "Collision.h"

bool Collision::isCollide(MyCollider* collider, const XMFLOAT3 & origin, const XMFLOAT3 & direction, float & dist)
{
	switch (collider->GetType())
	{
	case COLLIDER_TYPE::BOUNDING_BOX:
		return isCollide(static_cast<MyBOBox *>(collider)->GetBOBox(), origin, direction, dist);
		break;

	default:
		// 경우의 수가 없으므로 경고창을 뱉습니다.
		assert(false);
		return false;
		break;
	}
}

bool Collision::ProcessCollision(const BoundingOrientedBox& moveObject,
	const MyBOBox& staticObject,
	const XMFLOAT3& beforUpdatePosition,
	const XMFLOAT3& nowVelocity,
	float fElapsedTime,
	bool StaticObjIsMove,
	XMFLOAT3& SlideVector)
{
	// 이동한 박스를 통해 충돌한다.
	bool isAlreadyCollide = Collision::isCollide(moveObject, staticObject.GetBOBox());
	if (!isAlreadyCollide) return false;
	for (int x = 0; x < 4; ++x) //  plane 4 면에 대해 체크한다..
	{
		XMFLOAT3 intersectionPoint;
		// 여기서 d란... 원점과 평면과의 거리를 의미한다. (양수/음수)
		float d = 0;
		if (StaticObjIsMove)
		{
			d = Plane::CaculateD(staticObject.GetPlane(x), staticObject.GetPosOnPlane(x));
		}
		else
		{
			d = staticObject.GetPlane(x).w;
		}

		// 시간에 따른 이동을 기준으로 하므로 velocity 는 프레임 시간을 곱한다.
		bool isFront = Plane::IsFront(staticObject.GetPlaneNormal(x), d, beforUpdatePosition); // 업데이트 이전 위치
		// 만약 무한한 평면에 교차했다면...
		if (isFront)
		{
			bool isIntersect = Plane::Intersect(staticObject.GetPlaneNormal(x),
				d, beforUpdatePosition,
				Vector3::ScalarProduct(nowVelocity, fElapsedTime, false),
				intersectionPoint);

			if (isIntersect)
			{
				// 해당 교차점이 유한평면안에 존재하는지 확인한다.
				// staticObject[i].IsIn(x, intersectionPoint)
				// 
				if (Collision::isIn(staticObject.GetBOBox(), intersectionPoint)) //isIn 함수 나중에 수정해야함
				{
					SlideVector =
						Vector3::Sliding(staticObject.GetPlaneNormal(x), nowVelocity);
					return true;
				}
			}
		}
	}
	return false;
}

XMFLOAT3 Collision::GetCenter(const MyCollider& collider)
{  
	XMFLOAT3 center;
	switch (collider.GetType())
	{
	case BOUNDING_BOX:
		center = static_cast<const MyBOBox*>(&collider)->GetBOBox().Center;
		break;
	case BOUNDING_SPHERE:
		center = static_cast<const MyBSphere*>(&collider)->GetBSphere().Center;
		break;
	default:
		break;
	} 
	return center;
}
 
void Collision::ProcessCollideEdge(Player * player, int TerrainObjectCount, const MyBOBox * terrainObject)
{
	float fElapsedTime = CGameTimer::GetInstance()->GetTimeElapsed();
	BoundingOrientedBox AlreadyPlayerBBox = player->CalculateAlreadyBoundingBox(fElapsedTime);
	
	XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };
	bool isSlide = false;
	for (int i = 0; i < TerrainObjectCount; ++i)
	{
		isSlide = Collision::ProcessCollision(
			AlreadyPlayerBBox,
			terrainObject[i],
			player->GetTransform().GetPosition(),
			player->GetVelocity(),
			fElapsedTime,
			false,
			slideVector);

		if (isSlide)
		{ 
			player->SetVelocity(slideVector); 
		}
	}
	  
}
 
bool Collision::isCollide(const MyCollider& a, const MyCollider& b)
{
	COLLIDER_TYPE A_type = a.GetType();
	COLLIDER_TYPE B_type = b.GetType();

	auto t = &a;

	if (A_type == COLLIDER_TYPE::BOUNDING_BOX && B_type == COLLIDER_TYPE::BOUNDING_BOX)
	{
		BoundingOrientedBox collider1 = static_cast<const MyBOBox*>(&a)->GetBOBox();
		BoundingOrientedBox collider2 = static_cast<const MyBOBox*>(&b)->GetBOBox();
		return collider1.Intersects(collider2);
	}

	if (A_type == COLLIDER_TYPE::BOUNDING_BOX && B_type == COLLIDER_TYPE::BOUNDING_SPHERE)
	{
		BoundingOrientedBox collider1 = static_cast<const MyBOBox*>(&a)->GetBOBox();
		BoundingSphere collider2 = static_cast<const MyBSphere*>(&b)->GetBSphere();
		return collider1.Intersects(collider2);
	}
	if (A_type == COLLIDER_TYPE::BOUNDING_SPHERE && B_type == COLLIDER_TYPE::BOUNDING_BOX)
	{
		BoundingSphere collider1 = static_cast<const MyBSphere*>(&a)->GetBSphere();
		BoundingOrientedBox collider2 = static_cast<const MyBOBox*>(&b)->GetBOBox();
		return collider1.Intersects(collider2);
	}
	if (A_type == COLLIDER_TYPE::BOUNDING_SPHERE && B_type == COLLIDER_TYPE::BOUNDING_SPHERE)
	{
		BoundingSphere collider1 = static_cast<const MyBSphere*>(&a)->GetBSphere();
		BoundingSphere collider2 = static_cast<const MyBSphere*>(&b)->GetBSphere();
		return collider1.Intersects(collider2);
	}

	// 해당 경우의 수가 없으므로 경고창을 뱉습니다.
	assert(false);
}
 