#include "stdafx.h"
#include "TerrainObjectID.h"
 

TerrainObjectID & TerrainObjectID::operator=(const TerrainObjectID & other)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return *this;
}

TerrainObjectID::TerrainObjectID()
{
}

TerrainObjectID::~TerrainObjectID()
{
	assert(!(m_ID == ""));
}
 

std::ostream & operator<<(std::ostream & os, const TerrainObjectID & TerrainObjectID)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	os << TerrainObjectID.m_ID;
	return os;
}
