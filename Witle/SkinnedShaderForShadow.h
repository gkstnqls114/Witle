#pragma once
#include "ShadowShader.h"

class SkinnedShaderForShadow :
	public ShadowShader
{
	virtual D3D12_SHADER_BYTECODE		CreateVertexShader(ID3DBlob **ppd3dShaderBlob) override ;

public:
	SkinnedShaderForShadow();
	virtual ~SkinnedShaderForShadow();

};

