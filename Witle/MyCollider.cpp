#include "stdafx.h"
#include "MyCollider.h"
  
MyCollider::MyCollider(GameObject * pOwner, COLLIDER_TYPE type)
	: m_ColliderType(type) , ComponentBase(pOwner)
{
	
}

MyCollider::~MyCollider()
{
}
