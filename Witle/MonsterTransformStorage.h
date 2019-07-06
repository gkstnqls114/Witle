#pragma once
 
class QuadtreeTerrain;
class LoadObject;
class Texture;

struct VS_SRV_INSTANCEINFO;
/*

�ͷ��� ���� ��ġ�Ǵ� ������Ʈ���� ��Ƴ��� Ŭ�����̴�.
["�̸�"]["�ͷ��� ���̵�"]�� ���� �����ִ� ������������� ������ �� �ִ�.

*/
class MonsterTransformStorage
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
	 
	bool m_isCreate = false;  
	std::map<std::string, TerrainObjectInfo> m_MonsterTransformStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int count, XMFLOAT4X4* transforms);

private: 
	// ���� �ʱ� ��ġ ����
	void LoadMonsterFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName);
	void LoadNameAndPositionFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile);
	bool LoadTransform(char* name, const char* comp_name, const XMFLOAT4X4& tr);
	// ���� �ʱ� ��ġ ���� �ε�

	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);

public:
	static MonsterTransformStorage* m_Instance;

public:
	static MonsterTransformStorage* GetInstance(const QuadtreeTerrain const * pTerrain);
	static MonsterTransformStorage* GetInstance( );
	static void ReleaseInstance();

	void ReleaseObjects();
	int GetObjectCount(int index, const std::string& name);
	int GetObjectAllCount(int index);

	XMFLOAT4X4 GetWorldMatrix(int index, const std::string& name);
	XMFLOAT3 GetPosition(int index, const std::string& name);

	void CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	int Count(const std::string & name) ;
};