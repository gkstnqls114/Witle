#include "stdafx.h"
#include "BloomRevealShader.h"

D3D12_SHADER_BYTECODE BloomRevealShader::CreateComputeShader(ID3DBlob ** ppd3dShaderBlob)
{
	return ComputeShader::CompileShaderFromFile(L"BloomRevealShader.hlsl", "BloomReveal", ppd3dShaderBlob);
}

BloomRevealShader::BloomRevealShader()
{
}

BloomRevealShader::~BloomRevealShader()
{
}

void BloomRevealShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * const pd3dComputeRootSignature)
{
	ComputeShader::CreatePipelineState(pd3dDevice, pd3dComputeRootSignature);
}
