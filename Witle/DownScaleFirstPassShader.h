#pragma once
#include "ComputeShader.h"

class DownScaleFirstPassShader
	: public ComputeShader
{
	virtual D3D12_SHADER_BYTECODE CreateComputeShader(ID3DBlob ** ppd3dShaderBlob) override;

public:
	DownScaleFirstPassShader();
	virtual ~DownScaleFirstPassShader();

	virtual void						CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature* const pd3dComputeRootSignature) override;

};
