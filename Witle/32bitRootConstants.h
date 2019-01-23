#pragma once
#include "ResourceBase.h"

/*
���̴��� �����ϴ� ��Ʈ ��� Ŭ���� �Դϴ�.
*/ 
class RootConstants :
	public ResourceBase
{
	UINT m_parameterIndex{ 0 };
	UINT m_ResourceCount; // ��Ʈ����� �Ѱܾ��� ���ҽ� ����
	UINT *DestOffsets{ nullptr };

public:
	RootConstants(UINT paraIndex, UINT resourceCount, UINT StartOffset = 0);
	virtual ~RootConstants();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4 *resources) ;
	virtual void ReleaseShaderVariables() {};

};
 