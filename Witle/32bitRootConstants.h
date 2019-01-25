#pragma once
#include "ResourceBase.h"

/*
���̴��� �����ϴ� ��Ʈ ��� Ŭ���� �Դϴ�.
*/ 
class RootConstants :
	public ResourceBase
{ 
	UINT m_4ByteSize{ 0 };

public: 
	RootConstants(UINT paraIndex, UINT elementSize);
	virtual ~RootConstants();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) override {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const SourcePtr& data) override;
	virtual void ReleaseShaderVariables() override {};

};
 
