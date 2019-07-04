#pragma once
 
class QuadtreeTerrain;
class LoadObject;
class Texture;

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
		int         TerrainObjectCount{ 0 };					// �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ����
		std::vector<XMFLOAT4X4> TransformList;                  // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ���� ���
	};
	 
	struct RenderInfo // �׸��� �׸��� ���� �ʿ��� ��/�ؽ�������
	{
		LoadObject* pLoadObject{ nullptr };
		Texture* pTexture{ nullptr };
	};

	struct MonsterInfo // ���� �ʱ� ���� ��ġ ����
	{
		int MonsterCount{ 0 };
		std::vector<XMFLOAT4X4> TransformList; 
	};
	 
	bool m_isCreate = false;  
	std::map<std::string, RenderInfo> m_StaticObjectModelsStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	std::map<std::string, TerrainObjectInfo*> m_MonsterTransformStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	std::map<std::string, MonsterInfo*> m_MonsterStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int count, XMFLOAT4X4* transforms);

private: 
	// �ͷ��� �ʱ� ��ġ ����
	void LoadTerrainObjectFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName, const QuadtreeTerrain const * pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile, const QuadtreeTerrain const * pTerrain);
	bool LoadTransform(char* name, const char* comp_name, const XMINT4&, const XMFLOAT4X4& tr);
	// �ͷ��� �ʱ� ��ġ ���� �ε�

	
	// ���� �ʱ� ��ġ ���� �ε�

	void LoadMonsterTransform(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName);
	// �ͷ��� ����

	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);

public:
	static StaticObjectStorage* m_Instance;

public:
	static StaticObjectStorage* GetInstance(const QuadtreeTerrain const * pTerrain);
	static StaticObjectStorage* GetInstance( );
	static void ReleaseInstance();

	void ReleaseObjects();
	int GetObjectCount(int index, const std::string& name);
	int GetObjectAllCount(int index);

	XMFLOAT4X4* GetWorldMatrix(int index, const std::string& name);

	void CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QuadtreeTerrain const * pTerrain);
	
};