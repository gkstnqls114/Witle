#pragma once

class CLoadedModelInfo;
class CMesh;
class LoadObject;

/*

터레인 위에 배치되는 오브젝트들을 모아놓은 클래스이다.
터레인 오브젝트의 이름과 터레인 조각의 인덱스를 키로 가진다.

┌ m_StaticObjectStorage -------------------┐
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
		int         TerrainObjectCount = 0; // 터레인 조각 위에 배치되는 오브젝트의 개수
		XMFLOAT4X4* pTerrainObjectsTransform; // 터레인 조각 위에 배치되는 오브젝트의 월드 행렬
	};

	 
	bool m_isCreate = false;
	std::map<std::string, TerrainObjectInfo> m_StaticObjectStorage; // 모델 이름은 반드시 클래스에 맞춘다.
	
public:
	static StaticObjectStorage* m_Instance;

public:
	static StaticObjectStorage* GetInstance();

	void CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, int TerrainPieceCount);
	
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, int terrainID);
};