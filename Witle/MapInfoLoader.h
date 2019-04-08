#pragma once

class MapInfoLoader
{
private:
	MapInfoLoader() {};
	~MapInfoLoader() {};
	static void LoadNameAndPositionFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, FILE *pInFile);

public:
	static void LoadTerrainObjectFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, const char *pstrFileName);
	
};
 