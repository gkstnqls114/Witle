#pragma once
 
class QtTerrainInstancingDrawer;
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
		ID3D12Resource* m_pd3dcbGameObjects{ nullptr };         // �ν��Ͻ��� ���� ���Ǵ� ����
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects{ nullptr }; // �ν��Ͻ��� ���� ���Ǵ� ����
		int         TerrainObjectCount{ 0 };					// �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ����
		std::vector<XMFLOAT4X4> TransformList;                  // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ���� ���
	};
	 
	struct RenderInfo // �׸��� �׸��� ���� �ʿ��� ��/�ؽ�������
	{
		LoadObject* pLoadObject{ nullptr };
		Texture* pTexture{ nullptr };
	};
	 
	bool m_isCreate = false;  
	std::map<std::string, RenderInfo> m_StaticObjectModelsStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	std::map<std::string, TerrainObjectInfo*> m_StaticObjectStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	std::vector<XMFLOAT4X4> m_AltarTransformStorage; // Altar transform ��ġ �����ϴ� ��
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int count, XMFLOAT4X4* transforms);

private: 
	// �ͷ��� �ʱ� ��ġ ����
	void LoadTerrainObjectFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName, const QtTerrainInstancingDrawer const * pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile, const QtTerrainInstancingDrawer const * pTerrain);
	bool LoadTransform(char* name, const char* comp_name, const XMINT4&, const XMFLOAT4X4& tr);
	// �ͷ��� �ʱ� ��ġ ���� �ε�
	 
	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);

public:
	static StaticObjectStorage* m_Instance;

public:
	static StaticObjectStorage* GetInstance(const QtTerrainInstancingDrawer const * pTerrain);
	static StaticObjectStorage* GetInstance( );
	static void ReleaseInstance();

	void ReleaseObjects();
	int GetObjectCount(int index, const std::string& name);
	int GetObjectAllCount(int index);

	XMFLOAT4X4* GetWorldMatrix(int index, const std::string& name);
	XMFLOAT4X4 GetAltarTransform(int index, const std::string& name);

	void CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QtTerrainInstancingDrawer const * pTerrain);
	
	// �ν��Ͻ��� ���� �����մϴ�.
	void RenderAll(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers);
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, int index, bool isGBuffers);
	void RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList, int index, bool isGBuffers);
	void RenderBOBox(ID3D12GraphicsCommandList * pd3dCommandList, int index);
};