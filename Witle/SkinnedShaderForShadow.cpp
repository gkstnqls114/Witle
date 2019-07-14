#include "stdafx.h"
#include "SkinnedShaderForShadow.h"
 
SkinnedShaderForShadow::SkinnedShaderForShadow()
{
}

SkinnedShaderForShadow::~SkinnedShaderForShadow()
{
}
  
D3D12_SHADER_BYTECODE SkinnedShaderForShadow::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shadow.hlsl", "VSSkinned", "vs_5_1", ppd3dShaderBlob));
}
 