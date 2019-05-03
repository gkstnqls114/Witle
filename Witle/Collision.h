#pragma once

struct RAY;

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

	static bool isIn(const  BoundingOrientedBox& a, const XMFLOAT3& point)
	{
		return a.Contains(Vector3::XMFloat3ToVector(point)) == CONTAINS;
	}
	   
	static bool isCollide(const BoundingSphere& a, const BoundingOrientedBox& b)
	{
		return a.Intersects(b);
	}

};