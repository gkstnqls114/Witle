#include "stdafx.h"
#include "HorizonBlurShader.h"

D3D12_SHADER_BYTECODE HorizonBlurShader::CreateComputeShader(ID3DBlob ** ppd3dShaderBlob)
{
	return ComputeShader::CompileShaderFromFile(L"BlurShader.hlsl", "HorzCS", ppd3dShaderBlob);
}

HorizonBlurShader::HorizonBlurShader()
{
}

HorizonBlurShader::~HorizonBlurShader()
{
}

void HorizonBlurShader::CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature * const pd3dComputeRootSignature)
{
	ComputeShader::CreatePipelineState(pd3dDevice, pd3dComputeRootSignature);
}
