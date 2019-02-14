#include "stdafx.h"
#include "VerticalBlurShader.h"

D3D12_SHADER_BYTECODE VerticalBlurShader::CreateComputeShader(ID3DBlob ** ppd3dShaderBlob)
{
	return ComputeShader::CompileShaderFromFile(L"BlurShader.hlsl", "VertCS", ppd3dShaderBlob);
}

VerticalBlurShader::VerticalBlurShader()
{
}

VerticalBlurShader::~VerticalBlurShader()
{
}

void VerticalBlurShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * const pd3dComputeRootSignature)
{
	ComputeShader::CreatePipelineState(pd3dDevice, pd3dComputeRootSignature);
}
