#pragma once
#include "ResourceBase.h"

/*
쉐이더에 전달하는 루트 상수 클래스 입니다.
*/ 
class RootConstants :
	public ResourceBase
{
	UINT m_parameterIndex{ 0 };
	UINT m_ResourceCount; // 루트상수로 넘겨야할 리소스 개수
	UINT *DestOffsets{ nullptr };

public:
	RootConstants(UINT paraIndex, UINT resourceCount, UINT StartOffset = 0);
	virtual ~RootConstants();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4 *resources) ;
	virtual void ReleaseShaderVariables() {};

};
 