#pragma once
 
class QuadTreeTerrainMesh;

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
private:
	struct TerrainObjectInfo;
	int TerrainPieceCount;
	 
	struct TerrainObjectInfo
	{
		int         TerrainObjectCount = 0; // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ����
		//XMFLOAT4X4* pTerrainObjectsTransform; // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ���� ���
		std::list<XMFLOAT4X4> TransformList;
	};

	 
	bool m_isCreate = false;
	std::map<std::string, TerrainObjectInfo*> m_StaticObjectStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	
private:
	void LoadTerrainObjectFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName, const QuadTreeTerrainMesh const * pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile, const QuadTreeTerrainMesh const * pTerrain);

public:
	static StaticObjectStorage* m_Instance;

public:
	static StaticObjectStorage* GetInstance(const QuadTreeTerrainMesh const * pTerrain);

	void CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QuadTreeTerrainMesh const * pTerrain);
	
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, int terrainID);
};