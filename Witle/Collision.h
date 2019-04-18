#pragma once

class Collision
{
public:
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

};