#pragma once

class CLoadedModelInfo;
class CMesh;
class LoadObject;

/*

�ͷ��� ���� ��ġ�Ǵ� ������Ʈ���� ��Ƴ��� Ŭ�����̴�.
�ͷ��� ������Ʈ�� �̸��� �ͷ��� ������ �ε����� Ű�� ������.

�� m_StaticObjectStorage -------------------��
| name           |    pointer              |
|
|
|

*/
class StaticObjectStorage
{  
	struct TerrainObjectInfo;
	struct TerrainPiece
	{
		int                TerrainPieceCount;
		TerrainObjectInfo* pTerrainObject;
	};

	struct TerrainObjectInfo
	{
		int         TerrainObjectCount = 0; // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ����
		XMFLOAT4X4* pTerrainObjectsTransform; // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ���� ���
	};

	 
	bool m_isCreate = false;
	std::map<std::string, TerrainObjectInfo> m_StaticObjectStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	
public:
	static StaticObjectStorage* m_Instance;

public:
	static StaticObjectStorage* GetInstance();

	void CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, int TerrainPieceCount);
	
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, int terrainID);
};