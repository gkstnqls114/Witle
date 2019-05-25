#pragma once

class Player;

// ���� ��Ʈ �ñ״�ó�� �ϳ���� �����ϰ� �ִ�.
// �ش� ��Ʈ �ñ״�ó�� ���⼭ �����Ѵ�.
class GraphicsRootSignatureMgr
{
private:
	static ID3D12RootSignature* m_d3dGraphicsRootSignature;

	static ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
public:
	static void BuildObject(ID3D12Device *pd3dDevice);

	static ID3D12RootSignature* GetGraphicsRootSignature() { return m_d3dGraphicsRootSignature; }
};