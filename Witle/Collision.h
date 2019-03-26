#pragma once

class Collision
{
public:
	static bool isCollide(const BoundingBox& a, const BoundingBox& b)
	{
		return a.Intersects(b);
	}
};