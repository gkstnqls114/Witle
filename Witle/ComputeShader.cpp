#include "stdafx.h"
#include "ComputeShader.h"

ComputeShader::ComputeShader()
{
}

ComputeShader::~ComputeShader()
{
	if (m_PipelineState)
	{
		m_PipelineState->Release();
	}
}

D3D12_SHADER_BYTECODE ComputeShader::CompileShaderFromFile(const WCHAR * pszFileName, LPCSTR pszShaderName, ID3DBlob ** ppd3dShaderBlob)
{

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, "cs_5_1", nCompileFlags, 0, ppd3dShaderBlob, NULL);

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

	return(d3dShaderByteCode);
}

void ComputeShader::CreatePipelineState(ID3D12Device * pd3dDevice, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	ID3DBlob *pd3dComputeShaderBlob = nullptr;

	D3D12_COMPUTE_PIPELINE_STATE_DESC computepipelinestateDESC = {};

	computepipelinestateDESC.pRootSignature = pd3dGraphicsRootSignature;
	computepipelinestateDESC.CS = CreateComputeShader(&pd3dComputeShaderBlob);
	computepipelinestateDESC.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	computepipelinestateDESC.NodeMask = 0; // ´ÜÀÏ gpu : 0

	pd3dDevice->CreateComputePipelineState(&computepipelinestateDESC, IID_PPV_ARGS(&m_PipelineState));

	if (pd3dComputeShaderBlob) pd3dComputeShaderBlob->Release();

}

D3D12_SHADER_BYTECODE ComputeShader::CreateComputeShader(ID3DBlob ** ppd3dShaderBlob)
{
	return D3D12_SHADER_BYTECODE();
}

void ComputeShader::OnPrepareRender(ID3D12GraphicsCommandList * pd3dCommandList)
{
	pd3dCommandList->SetPipelineState(m_PipelineState);
}
