#pragma once

class ComputeShader
{
public:
	ComputeShader();
	virtual ~ComputeShader();

protected:
	D3D12_SHADER_BYTECODE				CompileShaderFromFile(const WCHAR *pszFileName, LPCSTR pszShaderName, ID3DBlob **ppd3dShaderBlob);
	void								CreatePipelineState(ID3D12Device *pd3dDevice, ID3D12RootSignature*pd3dGraphicsRootSignature);

	virtual D3D12_SHADER_BYTECODE		CreateComputeShader(ID3DBlob **ppd3dShaderBlob);
	
public:
	virtual void						CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature* const pd3dGraphicsRootSignature) = 0;
	
	const ID3D12RootSignature*			GetGraphicsRootSignature() const { return m_pd3dGraphicsRootSignature; }

	virtual void						OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);

	ID3D12PipelineState*  GetPSO() const { return m_PipelineState; }

protected:
	ID3D12PipelineState*			m_PipelineState{ nullptr };

	ID3D12RootSignature				*m_pd3dGraphicsRootSignature{ nullptr };
};
