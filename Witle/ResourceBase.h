#pragma once


class ResourceBase
{
protected:
	UINT m_parameterIndex{ 0 };
	const void * m_pData{ nullptr };
	UINT m_DataSize{ 0 };

public:
	ResourceBase();
	virtual ~ResourceBase();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) = 0;
	virtual void ReleaseShaderVariables() = 0;
};

