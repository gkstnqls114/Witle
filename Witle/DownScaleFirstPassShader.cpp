#include "stdafx.h"
#include "DownScaleFirstPassShader.h"

D3D12_SHADER_BYTECODE DownScaleFirstPassShader::CreateComputeShader(ID3DBlob ** ppd3dShaderBlob)
{
	return ComputeShader::CompileShaderFromFile(L"TonemappingComputeShader.hlsl", "DownScaleFirstPass", ppd3dShaderBlob);
}

DownScaleFirstPassShader::DownScaleFirstPassShader()
{
}

DownScaleFirstPassShader::~DownScaleFirstPassShader()
{
}

void DownScaleFirstPassShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * const pd3dComputeRootSignature)
{
	ComputeShader::CreatePipelineState(pd3dDevice, pd3dComputeRootSignature);
}
