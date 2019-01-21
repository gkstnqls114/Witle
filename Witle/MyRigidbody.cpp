#include "stdafx.h"
#include "MyRigidbody.h"

MyRigidbody::MyRigidbody(GameObject* pOwner)
	:ComponentBase(pOwner)
{
}

MyRigidbody::~MyRigidbody()
{

} 

void MyRigidbody::AddForce(const XMFLOAT3 & force)
{
}

void MyRigidbody::AddForce(float x, float y, float z)
{
}
