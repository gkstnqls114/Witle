#pragma once
 
class QuadtreeTerrain;
class LoadObject;

struct VS_SRV_INSTANCEINFO;
/*

터레인 위에 배치되는 오브젝트들을 모아놓은 클래스이다.
["이름"]["터레인 아이디"]를 통해 갖고있는 월드행렬정보를 보여줄 수 있다.

*/
class StaticObjectStorage
{  
private: 
	int TerrainPieceCount = 0; // 터레인 제일 작은 조각이 몇개 있는가?
	int TerrainObjectAllCount = 0; //모든 터레인 오브젝트는 몇개가 있는가?
	 
	struct TerrainObjectInfo
	{ 
		ID3D12Resource* m_pd3dcbGameObjects{ nullptr };
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects{ nullptr };
		int         TerrainObjectCount = 0; // 터레인 조각 위에 배치되는 오브젝트의 개수
		//XMFLOAT4X4* pTerrainObjectsTransform; // 터레인 조각 위에 배치되는 오브젝트의 월드 행렬
		std::vector<XMFLOAT4X4> TransformList;
	};
	 
	bool m_isCreate = false; 
	std::map<std::string, LoadObject*> m_StaticObjectModelsStorage; // 모델 이름은 반드시 클래스에 맞춘다.
	std::map<std::string, TerrainObjectInfo*> m_StaticObjectStorage; // 모델 이름은 반드시 클래스에 맞춘다.
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
	
	// 인스턴싱을 통해 렌더합니다.
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, int index);
};