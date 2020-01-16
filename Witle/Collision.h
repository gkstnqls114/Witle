#pragma once 
 
struct RAY;

class Player;
class Monster;
class MyCollider;
class MyBOBox; 

class Collision
{

public:
	static bool isCollide(const BoundingOrientedBox& box, const XMFLOAT3& origin, const XMFLOAT3& direction, float& dist)
	{
		return box.Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), dist);
	}

	static bool isCollide(const BoundingBox& box, const XMFLOAT3& origin, const XMFLOAT3& direction, float& dist)
	{
		return box.Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), dist);
	}

	static bool isCollide(const BoundingBox& a, const BoundingBox& b)
	{
		return a.Intersects(b);
	}

	static bool isCollide(const BoundingOrientedBox& a, const BoundingOrientedBox& b)
	{
		return a.Intersects(b);
	}

	static bool isCollide(const BoundingSphere& a, const BoundingOrientedBox& b)
	{
		return a.Intersects(b);
	}
	 
	static bool isCollide(const MyCollider& a, const MyCollider& b); 

	static bool isIn(const  BoundingOrientedBox& a, const XMFLOAT3& point)
	{
		return a.Contains(Vector3::XMFloat3ToVector(point)) == CONTAINS;
	}

	static bool isIn(const  BoundingBox& a, const XMFLOAT3& point)
	{
		return a.Contains(Vector3::XMFloat3ToVector(point)) == CONTAINS;
	}

	static bool isCollide(MyCollider* collider, const XMFLOAT3& origin, const XMFLOAT3& direction, float& dist);

	// 만약 MyBOBox에 부딪히면 슬라이딩 벡터로 처리한다.
	static bool ProcessCollision(const BoundingOrientedBox& moveObject,
		const MyBOBox& staticObject,  
		const XMFLOAT3 & beforUpdatePosition,
		const XMFLOAT3 & nowVelocity,
		float fElapsedTime,
		bool StaticObjIsMove,
		/*out*/XMFLOAT3& SlideVector);
	\

	static void ProcessCollideEdge(Player* player, int TerrainObjectCount, const MyBOBox* terrainObject);
	  
};