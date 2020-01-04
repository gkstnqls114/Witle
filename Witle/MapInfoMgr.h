#pragma once
 
// Map(= Terrian)에 대한 중심 위치와 가로 세로 사이즈를 관리한다.
// static 변수 관리. 인스턴스 생성할 필요 없음
class MapInfoMgr
{
	static XMFLOAT3 map_center; // 맵(= terrian)의 중심위치
	static XMFLOAT2 map_size;   // 맵(= terrian))의 x, z 사이즈 

private:
	MapInfoMgr() = delete;
	~MapInfoMgr() = delete;

public: 
	static XMFLOAT2 GetMapSize() { return map_size; }
	static float GetMapSizeX() { return map_size.x; }
	static float GetMapSizeZ() { return map_size.y; }
	static float GetMapExtentsX() { return map_size.x / 2.f; } // 반지름 사이즈 리턴
	static float GetMapExtentsZ() { return map_size.y / 2.f; } // 반지름 사이즈 리턴
	static XMFLOAT3 GetMapCenter() { return map_center; }
};
