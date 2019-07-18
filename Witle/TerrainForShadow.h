#pragma once
#include "ShadowShader.h"

class TerrainForShadow :
	public ShadowShader
{
	virtual D3D12_INPUT_LAYOUT_DESC		CreateInputLayout() override ;
	virtual D3D12_SHADER_BYTECODE		CreateVertexShader(ID3DBlob **ppd3dShaderBlob) override ;

public:
	TerrainForShadow();
	virtual ~TerrainForShadow();

};

