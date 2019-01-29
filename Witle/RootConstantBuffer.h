#pragma once
#include "ResourceBase.h"

class RootConstantBuffer
	: public ResourceBase
{
	ID3D12Resource* m_pResource{ nullptr };

public:
	RootConstantBuffer();
	virtual ~RootConstantBuffer();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) override; 
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const SourcePtr& data) override;
	virtual void ReleaseShaderVariables() override;
};