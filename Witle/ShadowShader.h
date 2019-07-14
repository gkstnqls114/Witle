#pragma once
#include "Shader.h"
class ShadowShader :
	public Shader
{
	// PSO �� ������ �� �ʿ��� ���ڵ�
	virtual D3D12_INPUT_LAYOUT_DESC		CreateInputLayout() override = 0;
	virtual D3D12_RASTERIZER_DESC		CreateRasterizerState() override;
	virtual D3D12_BLEND_DESC			CreateBlendState() override;
	virtual D3D12_DEPTH_STENCIL_DESC	CreateDepthStencilState() override;

	virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE	CreatePrimitiveTopologyType() override;

	virtual D3D12_SHADER_BYTECODE		CreateVertexShader(ID3DBlob **ppd3dShaderBlob) override = 0;
	virtual D3D12_SHADER_BYTECODE		CreatePixelShader(ID3DBlob **ppd3dShaderBlob) override;
	virtual D3D12_SHADER_BYTECODE		CreatePixelShaderForGBuffers(ID3DBlob **ppd3dShaderBlob) override;
	virtual D3D12_SHADER_BYTECODE		CreateGeometryShader(ID3DBlob **ppd3dShaderBlob) override;
	// PSO �� ������ �� �ʿ��� ���ڵ�

	virtual DXGI_FORMAT GetDSVFormat() override
	{
		return DXGI_FORMAT_D16_UNORM;
	};

public:
	ShadowShader();
	virtual ~ShadowShader();

	virtual void						CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature*const pd3dGraphicsRootSignature) override;
	virtual void						Update(float ElapsedTime) override;

};

