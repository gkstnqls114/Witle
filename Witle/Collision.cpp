#include "stdafx.h"
#include "MyCollider.h"
#include "MyBOBox.h"
#include "MyBSphere.h"
#include "Player.h"
#include "ModelStorage.h"
#include "StaticObjectStorage.h"
#include "QuadtreeTerrain.h"
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

void Collision::ProcessCollideEdge(Player * player, int TerrainObjectCount, const MyBOBox * terrainObject, float fElapsedTime)
{
	BoundingOrientedBox AlreadyPlayerBBox = player->CalculateAlreadyBoundingBox(fElapsedTime);
	XMFLOAT3 AlreadyPositon{ AlreadyPlayerBBox.Center.x, AlreadyPlayerBBox.Center.y, AlreadyPlayerBBox.Center.z };
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

void Collision::ProcessCollideTerrainObject(Player * player, const QuadtreeTerrain * quadTerrain, float fElapsedTime)
{ 
	BoundingOrientedBox AlreadyPlayerBBox = player->CalculateAlreadyBoundingBox(fElapsedTime);
	XMFLOAT3 AlreadyPositon{ AlreadyPlayerBBox.Center.x, AlreadyPlayerBBox.Center.y, AlreadyPlayerBBox.Center.z };

	XMINT4 IDs = quadTerrain->GetIDs(AlreadyPositon);
	int TerrainCount = quadTerrain->GetTerrainPieceCount();

	// Ti: Terrain Index
	for (int Ti = 0; Ti < TerrainCount; ++Ti)
	{
		int TerrainIndex = Ti;
		//if (Ti == 0) TerrainIndex = IDs.x;
		//else if (Ti == 1) TerrainIndex = IDs.y;
		//else if (Ti == 2) TerrainIndex = IDs.z;
		//else if (Ti == 3) TerrainIndex = IDs.w;

		//if (TerrainIndex == -1) continue;

		for (const auto& name : ModelStorage::GetInstance()->m_NameList)
		{
			MyBOBox* box = ModelStorage::GetInstance()->GetBOBox(name);
			if (!box) continue; // �浹�ڽ��� ���ٸ� �ٸ� ������Ʈ�� �˻�����.

			XMFLOAT4X4* pWorldMatrix = StaticObjectStorage::GetInstance(quadTerrain)->GetWorldMatrix(TerrainIndex, name);

			// Ʈ���� ���� ���� ������Ʈ ������ŭ �浹 üũ
			for (int i = 0; i < StaticObjectStorage::GetInstance(quadTerrain)->GetObjectCount(TerrainIndex, name); ++i)
			{
				//���� ��� ����´�.

				// �� �浹�ڽ��� ������� ���Ѵ�. �ϴ� ����� ���������� �̵�
				MyBOBox worldBox = *box;
				worldBox.Move(XMFLOAT3(pWorldMatrix[i]._41, 0, pWorldMatrix[i]._43));

				XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };

				bool isSlide = Collision::ProcessCollision(
					AlreadyPlayerBBox,
					worldBox,
					player->GetTransform().GetPosition(),
					player->GetVelocity(),
					fElapsedTime,
					true,
					slideVector);

				if (isSlide)
				{
					player->SetVelocity(slideVector);
					return;
				}

			}
		}
	} 
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
