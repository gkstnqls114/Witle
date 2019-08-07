#include "stdafx.h"
#include "DownScaleSecondPassShader.h"

D3D12_SHADER_BYTECODE DownScaleSecondPassShader::CreateComputeShader(ID3DBlob ** ppd3dShaderBlob)
{
	return ComputeShader::CompileShaderFromFile(L"TonemappingComputeShader.hlsl", "DownScaleSecondPass", ppd3dShaderBlob);
}

DownScaleSecondPassShader::DownScaleSecondPassShader()
{
}

DownScaleSecondPassShader::~DownScaleSecondPassShader()
{
}

void DownScaleSecondPassShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * const pd3dComputeRootSignature)
{
	ComputeShader::CreatePipelineState(pd3dDevice, pd3dComputeRootSignature);
}
