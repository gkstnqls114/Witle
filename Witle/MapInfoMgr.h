#pragma once
 
// Map(= Terrian)�� ���� �߽� ��ġ�� ���� ���� ����� �����Ѵ�.
// static ���� ����. �ν��Ͻ� ������ �ʿ� ����
class MapInfoMgr
{
	static XMFLOAT3 map_center; // ��(= terrian)�� �߽���ġ
	static XMFLOAT2 map_size;   // ��(= terrian))�� x, z ������ 

private:
	MapInfoMgr() = delete;
	~MapInfoMgr() = delete;

public: 
	static XMFLOAT2 GetMapSize() { return map_size; }
	static float GetMapSizeX() { return map_size.x; }
	static float GetMapSizeZ() { return map_size.y; }
	static float GetMapExtentsX() { return map_size.x / 2.f; } // ������ ������ ����
	static float GetMapExtentsZ() { return map_size.y / 2.f; } // ������ ������ ����
	static XMFLOAT3 GetMapCenter() { return map_center; }
};
