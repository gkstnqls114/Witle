#include "stdafx.h"
#include "FollowCam.h"


FollowCam::FollowCam()
{
}


FollowCam::~FollowCam()
{
}

void FollowCam::Move(const XMFLOAT3 & Shift)
{
	if (m_pTarget) {
		MoveAroundLookingobject(Shift);
	} 
}

void FollowCam::Rotate(float x, float y, float z)
{
	if (m_pTarget) {
		RotateOnLookingobject(x, y, z);
	}
}
