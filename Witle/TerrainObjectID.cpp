#include "stdafx.h"
#include "TerrainObjectID.h"
 

TerrainObjectID & TerrainObjectID::operator=(const TerrainObjectID & other)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
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
	// TODO: 여기에 반환 구문을 삽입합니다.
	os << TerrainObjectID.m_ID;
	return os;
}
