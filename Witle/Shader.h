#pragma once

class CTexture;

class IShader
{

};

class Shader
{
public:
	Shader();
	virtual ~Shader();
	 
protected:
	D3D12_SHADER_BYTECODE				CompileShaderFromFile(const WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);
	void								CreatePipelineState(ID3D12Device *pd3dDevice, ID3D12RootSignature*pd3dGraphicsRootSignature);
	void								CreatePipelineStateForGBuffers(ID3D12Device *pd3dDevice, ID3D12RootSignature*pd3dGraphicsRootSignature);

	virtual D3D12_INPUT_LAYOUT_DESC		CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC		CreateRasterizerState();
	virtual D3D12_BLEND_DESC			CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC	CreateDepthStencilState();

	virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE CreatePrimitiveTopologyType();

	virtual D3D12_SHADER_BYTECODE		CreateVertexShader(ID3DBlob **ppd3dShaderBlob) ;
	virtual D3D12_SHADER_BYTECODE		CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE		CreatePixelShaderForGBuffers(ID3DBlob **ppd3dShaderBlob) = 0;
	virtual D3D12_SHADER_BYTECODE		CreateGeometryShader(ID3DBlob **ppd3dShaderBlob) ; 

public:  
	virtual void						CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature* const pd3dGraphicsRootSignature) = 0;
 
	virtual void						OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void						OnPrepareRenderForGBuffers(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void						Update(float ElapsedTime) = 0;
	ID3D12PipelineState*                GetPSO() const { return m_PipelineState; }
	void                                ReleaseObjects();

protected:
	ID3D12PipelineState*			m_PipelineState{ nullptr };
	ID3D12PipelineState*			m_PipelineStateForGBuffers{ nullptr };
};
