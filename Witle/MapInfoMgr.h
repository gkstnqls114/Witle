#pragma once
 
// Map(= Terrian)�� ���� �߽� ��ġ�� ���� ���� ����� �����Ѵ�.
// static ���� ����. �ν��Ͻ� ������ �ʿ� ����
class MapInfoMgr
{
	static XMFLOAT3 map_center; // ��(= terrian)�� �߽���ġ
	static XMFLOAT2 map_size;   // ��(= terrian))�� ���� ���� ������

private:
	MapInfoMgr() = delete;
	~MapInfoMgr() = delete;

public: 
	static XMFLOAT2 GetMapSize() { return map_size; }
	static float GetMapSizeX() { return map_size.x; }
	static float GetMapSizeY() { return map_size.y; }
	static XMFLOAT3 GetMapCenter() { return map_center; }
};

// Build ������ ���⼭ Map Size ����
XMFLOAT3 MapInfoMgr::map_center{ 15000.f, 0.f, 15000.f }; 
XMFLOAT2 MapInfoMgr::map_size{ 30000.f, 30000.f };