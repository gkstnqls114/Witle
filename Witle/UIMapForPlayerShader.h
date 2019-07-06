#pragma once
#include "Shader.h"
class UIMapForPlayerShader :
	public Shader
{
	// PSO 를 생성할 때 필요한 인자들
	virtual D3D12_INPUT_LAYOUT_DESC		CreateInputLayout() override;
	virtual D3D12_RASTERIZER_DESC		CreateRasterizerState() override;
	virtual D3D12_BLEND_DESC			CreateBlendState() override;
	virtual D3D12_DEPTH_STENCIL_DESC	CreateDepthStencilState() override;

	virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE	CreatePrimitiveTopologyType() override;

	virtual D3D12_SHADER_BYTECODE		CreateVertexShader(ID3DBlob **ppd3dShaderBlob) override;
	virtual D3D12_SHADER_BYTECODE		CreatePixelShader(ID3DBlob **ppd3dShaderBlob) override;
	virtual D3D12_SHADER_BYTECODE		CreatePixelShaderForGBuffers(ID3DBlob **ppd3dShaderBlob) override;
	virtual D3D12_SHADER_BYTECODE		CreateGeometryShader(ID3DBlob **ppd3dShaderBlob) override;
	// PSO 를 생성할 때 필요한 인자들


public:
	UIMapForPlayerShader();
	virtual ~UIMapForPlayerShader();

	virtual void						CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature*const pd3dGraphicsRootSignature) override;
	virtual void						Update(float ElapsedTime) override;

};

