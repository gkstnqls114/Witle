#pragma once
//#include "GameObject.h"
//#include "Camera.h"

class CTexture;

class Shader
{
public:
	Shader();
	virtual ~Shader();
	 
protected:
	D3D12_SHADER_BYTECODE				CompileShaderFromFile(const WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);
	void								CreatePipelineState(ID3D12Device *pd3dDevice, ID3D12RootSignature*pd3dGraphicsRootSignature);

	virtual D3D12_INPUT_LAYOUT_DESC		CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC		CreateRasterizerState();
	virtual D3D12_BLEND_DESC			CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC	CreateDepthStencilState();

	virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE CreatePrimitiveTopologyType();

	virtual D3D12_SHADER_BYTECODE		CreateVertexShader(ID3DBlob **ppd3dShaderBlob) ;
	virtual D3D12_SHADER_BYTECODE		CreatePixelShader(ID3DBlob **ppd3dShaderBlob) ;
	virtual D3D12_SHADER_BYTECODE		CreateGeometryShader(ID3DBlob **ppd3dShaderBlob) ; 

public:  
	virtual void						CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature* const pd3dGraphicsRootSignature) = 0;
	virtual void						SetDescriptorHeaps(ID3D12GraphicsCommandList * pd3dCommandList);
	virtual void						SetGraphicsRootSignature(ID3D12GraphicsCommandList * pd3dCommandList);

	virtual void						CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	const ID3D12RootSignature*			GetGraphicsRootSignature() const { return m_pd3dGraphicsRootSignature; }
	 
	virtual void						OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList); 

	virtual void						Update(float ElapsedTime) = 0;
	ID3D12PipelineState*  GetPSO() const { return m_pd3dPipelineState; }

protected:
	ID3D12PipelineState*			m_pd3dPipelineState{ nullptr };

	ID3D12DescriptorHeap			*m_pd3dDescriptorHeap{ nullptr };
	ID3D12RootSignature				*m_pd3dGraphicsRootSignature{ nullptr };
};
