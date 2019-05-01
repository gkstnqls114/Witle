#include "stdafx.h"
#include "FamilyID.h"

#define FAMILYID_MESH "Mesh"
#define FAMILYID_LIGHT "Light"
#define FAMILYID_TRANSFORM "Transform"
#define FAMILYID_CAMERA "Camera"
#define FAMILYID_FRUSTUM "Frustum"
#define FAMILYID_TEST "TEST"
#define FAMILYID_SKINNEDMESH "SkinnedMesh"
#define FAMILYID_SHAPE "Shape"

FamilyID & FamilyID::operator=(const FamilyID & other)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return *this;
}

FamilyID::FamilyID()
{
}

FamilyID::~FamilyID()
{
	// assert(!(m_ID == ""));
}

void FamilyID::InitTEST()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_TEST;
}

bool FamilyID::isTEST()
{
	if (m_ID == FAMILYID_TEST) return true;
	else return false;
}

void FamilyID::InitMesh()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_MESH;
}

void FamilyID::InitLight()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_LIGHT;
}

void FamilyID::InitTransform()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_TRANSFORM;
}

void FamilyID::InitCamera()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_CAMERA;
}

void FamilyID::InitFrustum()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_FRUSTUM;
}

void FamilyID::InitSkinnedMesh()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_SKINNEDMESH;
}

void FamilyID::InitShape()
{
	assert(!(m_ID != "")); // �̹� �ѹ� ������ ���¶�� �� �̻� �ٽ� ������ �� ����.
	m_ID = FAMILYID_SHAPE; 
}

bool FamilyID::isMesh() const
{
	if (m_ID == FAMILYID_MESH) return true;
	else return false;
}

bool FamilyID::isLight() const
{
	if (m_ID == FAMILYID_LIGHT) return true;
	else return false;
}

bool FamilyID::isTransform() const
{
	if (m_ID == FAMILYID_TRANSFORM) return true;
	else return false;
}

bool FamilyID::isCamera() const
{
	if (m_ID == FAMILYID_CAMERA) return true;
	else return false;
}

bool FamilyID::isFrustum() const
{
	if (m_ID == FAMILYID_FRUSTUM) return true;
	else return false;
}

bool FamilyID::isSkinnedMesh() const
{
	if (m_ID == FAMILYID_SKINNEDMESH) return true;
	return false;
}

bool FamilyID::isShape() const
{
	if (m_ID == FAMILYID_SHAPE) return true;
	return false;
}

std::ostream & operator<<(std::ostream & os, const FamilyID & familyID)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	os << familyID.m_ID;
	return os;
}
