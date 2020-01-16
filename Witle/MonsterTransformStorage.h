#pragma once
 
class QtTerrainInstancingDrawer;
class LoadObject;
class Texture;

struct VS_SRV_INSTANCEINFO;
/*

터레인 위에 배치되는 오브젝트들을 모아놓은 클래스이다.
["이름"]["터레인 아이디"]를 통해 갖고있는 월드행렬정보를 보여줄 수 있다.

*/
class MonsterTransformStorage
{  
private: 
	int TerrainPieceCount = 0; // 터레인 제일 작은 조각이 몇개 있는가?
	int TerrainObjectAllCount = 0; //모든 터레인 오브젝트는 몇개가 있는가?
	  
	struct TerrainObjectInfo
	{ 
		int         TerrainObjectCount{ 0 };					// 터레인 조각 위에 배치되는 오브젝트의 개수
		std::vector<XMFLOAT4X4> TransformList;                  // 터레인 조각 위에 배치되는 오브젝트의 월드 행렬
	};
	 
	struct RenderInfo // 그림을 그리기 위해 필요한 모델/텍스쳐정보
	{
		LoadObject* pLoadObject{ nullptr };
		Texture* pTexture{ nullptr };
	};
	 
	bool m_isCreate = false;  
	std::map<std::string, TerrainObjectInfo> m_MonsterTransformStorage; // 모델 이름은 반드시 클래스에 맞춘다.
	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int count, XMFLOAT4X4* transforms);

private: 
	// 몬스터 초기 위치 정보
	void LoadMonsterFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName);
	void LoadNameAndPositionFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile);
	bool LoadTransform(char* name, const char* comp_name, const XMFLOAT4X4& tr);
	// 몬스터 초기 위치 정보 로드

	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);

public:
	static MonsterTransformStorage* m_Instance;

public:
	static MonsterTransformStorage* GetInstance(const QtTerrainInstancingDrawer const * pTerrain);
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