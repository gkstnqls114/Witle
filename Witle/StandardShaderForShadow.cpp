#include "stdafx.h"
#include "StandardShaderForShadow.h"

StandardShaderForShadow::StandardShaderForShadow()
{
}

StandardShaderForShadow::~StandardShaderForShadow()
{
}

D3D12_SHADER_BYTECODE StandardShaderForShadow::CreateVertexShader(ID3DBlob ** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shadow.hlsl", "VSStandard", "vs_5_1", ppd3dShaderBlob));
}
