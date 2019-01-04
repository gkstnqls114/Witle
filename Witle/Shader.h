#pragma once
//#include "GameObject.h"
//#include "MyCamera.h"

class CTexture;

class Shader
{
public:
	Shader();
	virtual ~Shader();

private:
	int m_nReferences = 0;

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
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual void						BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList*pd3dCommandList) = 0;
	virtual void						AnimateObjects(float fTimeElapsed) = 0;
	virtual void						ReleaseObjects() = 0;
	virtual void						ReleaseUploadBuffers() = 0;

	virtual void						CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature*pd3dGraphicsRootSignature) = 0;
	virtual void						SetDescriptorHeaps(ID3D12GraphicsCommandList * pd3dCommandList);
	virtual void						SetGraphicsRootSignature(ID3D12GraphicsCommandList * pd3dCommandList);

	virtual void						CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature*	GetGraphicsRootSignature() const {
		return m_pd3dGraphicsRootSignature;
	}

	virtual void						CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) = 0;
	// virtual void						UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, MyCamera* pCamera = nullptr) = 0;
	virtual void						ReleaseShaderVariables() = 0;
	
	virtual void						UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	
	virtual void						OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	// virtual void						Render(ID3D12GraphicsCommandList *pd3dCommandList, MyCamera *pCamera) = 0;
	
	virtual void						Update(float ElapsedTime) = 0;

protected:
	ComPtr<ID3D12PipelineState>		m_pd3dPipelineState{ nullptr };

	ID3D12DescriptorHeap			*m_pd3dDescriptorHeap = NULL;
	ID3D12RootSignature				*m_pd3dGraphicsRootSignature = NULL;


};
