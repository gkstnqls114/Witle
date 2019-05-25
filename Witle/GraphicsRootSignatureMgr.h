#pragma once

class Player;

// 현재 루트 시그니처를 하나라고 가정하고 있다.
// 해당 루트 시그니처를 여기서 관리한다.
class GraphicsRootSignatureMgr
{
private:
	static ID3D12RootSignature* m_d3dGraphicsRootSignature;

	static ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
public:
	static void BuildObject(ID3D12Device *pd3dDevice);

	static ID3D12RootSignature* GetGraphicsRootSignature() { return m_d3dGraphicsRootSignature; }
};