#pragma once


class ResourceBase
{
protected:

public:
	ResourceBase();
	virtual ~ResourceBase();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const SourcePtr& data) = 0;
	virtual void ReleaseShaderVariables() = 0;
};

