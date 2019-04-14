#pragma once
 
class QuadtreeTerrain;
class LoadObject;

struct VS_SRV_INSTANCEINFO;
/*

�ͷ��� ���� ��ġ�Ǵ� ������Ʈ���� ��Ƴ��� Ŭ�����̴�.
["�̸�"]["�ͷ��� ���̵�"]�� ���� �����ִ� ������������� ������ �� �ִ�.

*/
class StaticObjectStorage
{  
private: 
	int TerrainPieceCount = 0; // �ͷ��� ���� ���� ������ � �ִ°�?
	int TerrainObjectAllCount = 0; //��� �ͷ��� ������Ʈ�� ��� �ִ°�?
	 
	struct TerrainObjectInfo
	{ 
		ID3D12Resource* m_pd3dcbGameObjects{ nullptr };
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects{ nullptr };
		int         TerrainObjectCount = 0; // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ����
		//XMFLOAT4X4* pTerrainObjectsTransform; // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ���� ���
		std::vector<XMFLOAT4X4> TransformList;
	};
	 
	bool m_isCreate = false; 
	std::map<std::string, LoadObject*> m_StaticObjectModelsStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	std::map<std::string, TerrainObjectInfo*> m_StaticObjectStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int count, XMFLOAT4X4* transforms);

private:
	void LoadTerrainObjectFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName, const QuadtreeTerrain const * pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile, const QuadtreeTerrain const * pTerrain);
	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);

public:
	static StaticObjectStorage* m_Instance;

public:
	static StaticObjectStorage* GetInstance(const QuadtreeTerrain const * pTerrain);

	void CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QuadtreeTerrain const * pTerrain);
	
	// �ν��Ͻ��� ���� �����մϴ�.
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, int index);
};