#pragma once
#include "ShadowShader.h"

class PlayerShadowSkinnedShader :
	public ShadowShader
{
	virtual D3D12_INPUT_LAYOUT_DESC		CreateInputLayout() override ;
	virtual D3D12_SHADER_BYTECODE		CreateVertexShader(ID3DBlob **ppd3dShaderBlob) override ;

public:
	PlayerShadowSkinnedShader();
	virtual ~PlayerShadowSkinnedShader();

};

