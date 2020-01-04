#pragma once
 
// Map(= Terrian)에 대한 중심 위치와 가로 세로 사이즈를 관리한다.
// static 변수 관리. 인스턴스 생성할 필요 없음
class MapInfoMgr
{
	static XMFLOAT3 map_center; // 맵(= terrian)의 중심위치
	static XMFLOAT2 map_size;   // 맵(= terrian))의 가로 세로 사이즈

private:
	MapInfoMgr() = delete;
	~MapInfoMgr() = delete;

public: 
	static XMFLOAT2 GetMapSize() { return map_size; }
	static float GetMapSizeX() { return map_size.x; }
	static float GetMapSizeY() { return map_size.y; }
	static XMFLOAT3 GetMapCenter() { return map_center; }
};

// Build 이전에 여기서 Map Size 관리
XMFLOAT3 MapInfoMgr::map_center{ 15000.f, 0.f, 15000.f }; 
XMFLOAT2 MapInfoMgr::map_size{ 30000.f, 30000.f };