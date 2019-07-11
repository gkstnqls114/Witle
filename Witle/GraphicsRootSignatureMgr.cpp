#include "stdafx.h"
#include "d3dUtil.h"
#include "GraphicsRootSignatureMgr.h"

ID3D12RootSignature* GraphicsRootSignatureMgr::m_d3dGraphicsRootSignature{ nullptr };

ID3D12RootSignature * GraphicsRootSignatureMgr::CreateGraphicsRootSignature(ID3D12Device * pd3dDevice)
{

	HRESULT hResult = S_FALSE;
	ID3D12RootSignature* pd3dGraphicsRootSignature = nullptr;

	//// 루트 패러미터 ///////////////////////////////////////////////////////////////////// 
	D3D12_ROOT_PARAMETER pRootParameters[ROOTPATAMETER_COUNT];

	pRootParameters[ROOTPARAMETER_WORLD] = d3dUtil::CreateRootParameterConstants(16, 0); // b0: World
	pRootParameters[ROOTPARAMETER_CAMERA] = d3dUtil::CreateRootParameterCBV(1);          // b1: Camera
	pRootParameters[ROOTPARAMETER_MATERIALS] = d3dUtil::CreateRootParameterCBV(2);       // b2: Materials
	pRootParameters[ROOTPARAMETER_LIGHTS] = d3dUtil::CreateRootParameterCBV(3);          // b3: Lights
	pRootParameters[ROOTPARAMETER_PICKINGPOINT] = d3dUtil::CreateRootParameterConstants(2, 4);  // b4: Color
	pRootParameters[ROOTPARAMETER_HPPERCENTAGE] = d3dUtil::CreateRootParameterConstants(1, 5);  // b5: HP percentage
	pRootParameters[ROOTPARAMETER_TIME] = d3dUtil::CreateRootParameterConstants(1, 6);  // b6: Elapsed Time

	D3D12_DESCRIPTOR_RANGE pTextureDescriptorRanges[3];
	pTextureDescriptorRanges[0] = d3dUtil::CreateDescriptorRangeSRV(1, 0); //t0: gtxtTexture
	pTextureDescriptorRanges[1] = d3dUtil::CreateDescriptorRangeSRV(1, 1); //t1: gtxtTerrainBaseTexture
	pTextureDescriptorRanges[2] = d3dUtil::CreateDescriptorRangeSRV(1, 2); //t2: gtxtTerrainDetailTexture	

	// 다시 루프 패러미터 
	pRootParameters[ROOTPARAMETER_TEXTURE] = d3dUtil::CreateRootParameterTable(1, &pTextureDescriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_TEXTUREBASE] = d3dUtil::CreateRootParameterTable(1, &pTextureDescriptorRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_TEXTUREDETAIL] = d3dUtil::CreateRootParameterTable(1, &pTextureDescriptorRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[8];
	pd3dDescriptorRanges[0] = d3dUtil::CreateDescriptorRangeSRV(1, 6);  //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[1] = d3dUtil::CreateDescriptorRangeSRV(1, 7);  //t7: gtxtSpecularTexture
	pd3dDescriptorRanges[2] = d3dUtil::CreateDescriptorRangeSRV(1, 8);  //t8: gtxtNormalTexture
	pd3dDescriptorRanges[3] = d3dUtil::CreateDescriptorRangeSRV(1, 9);  //t9: gtxtMetallicTexture
	pd3dDescriptorRanges[4] = d3dUtil::CreateDescriptorRangeSRV(1, 10); //t10: gtxtEmissionTexture
	pd3dDescriptorRanges[5] = d3dUtil::CreateDescriptorRangeSRV(1, 11); //t11: gtxtEmissionTexture
	pd3dDescriptorRanges[6] = d3dUtil::CreateDescriptorRangeSRV(1, 12); //t12: gtxtEmissionTexture
	pd3dDescriptorRanges[7] = d3dUtil::CreateDescriptorRangeSRV(1, 13); //t13: gtxtSkyBoxTexture

	pRootParameters[ROOTPARAMETER_ALBEDO] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_SPECULAR] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_NORMAL] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_METALLIC] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[3], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_EMISSION_1] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[4], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_EMISSION_2] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[5], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_EMISSION_3] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[6], D3D12_SHADER_VISIBILITY_PIXEL);

	pRootParameters[ROOTPARAMETER_SKINNEDBONEOFFSET] = d3dUtil::CreateRootParameterCBV(7, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	pRootParameters[ROOTPARAMETER_SKINNEDBONETRANSFORM] = d3dUtil::CreateRootParameterCBV(8, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	pRootParameters[ROOTPARAMETER_INSTANCING] = d3dUtil::CreateRootParameterSRV(14, 0, D3D12_SHADER_VISIBILITY_VERTEX); //t14: Instacing World
	pRootParameters[ROOTPARAMETER_SKYBOX] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[7], D3D12_SHADER_VISIBILITY_PIXEL); //t15: SKYBOX

	//// 루트 패러미터 ///////////////////////////////////////////////////////////////////// 


	//// 샘플러 ///////////////////////////////////////////////////////////////////// 
	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc[3];
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // warp shader
	d3dSamplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].MipLODBias = 0;
	d3dSamplerDesc[0].MaxAnisotropy = 1;
	d3dSamplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc[0].MinLOD = 0;
	d3dSamplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[0].ShaderRegister = 0;
	d3dSamplerDesc[0].RegisterSpace = 0;
	d3dSamplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	d3dSamplerDesc[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // clamp shader
	d3dSamplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].MipLODBias = 0;
	d3dSamplerDesc[1].MaxAnisotropy = 1;
	d3dSamplerDesc[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc[1].MinLOD = 0;
	d3dSamplerDesc[1].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[1].ShaderRegister = 1;
	d3dSamplerDesc[1].RegisterSpace = 0;
	d3dSamplerDesc[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	 
	d3dSamplerDesc[2].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; // clamp shader
	d3dSamplerDesc[2].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	d3dSamplerDesc[2].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	d3dSamplerDesc[2].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	d3dSamplerDesc[2].MipLODBias = 0;
	d3dSamplerDesc[2].MaxAnisotropy = 1;
	d3dSamplerDesc[2].ComparisonFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dSamplerDesc[2].MinLOD = 0;
	d3dSamplerDesc[2].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[2].ShaderRegister = 2;
	d3dSamplerDesc[2].RegisterSpace = 0;
	d3dSamplerDesc[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	d3dSamplerDesc[2].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	//// 샘플러 ///////////////////////////////////////////////////////////////////// 

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	// D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	//D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS; 

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pRootParameters);
	d3dRootSignatureDesc.pParameters = pRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(d3dSamplerDesc);
	d3dRootSignatureDesc.pStaticSamplers = d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = nullptr;
	ID3DBlob *pd3dErrorBlob = nullptr;
	hResult = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	assert(hResult == S_OK);

	hResult = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&pd3dGraphicsRootSignature));
	assert(hResult == S_OK);

	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
	 
	return (pd3dGraphicsRootSignature);
}

void GraphicsRootSignatureMgr::BuildObject(ID3D12Device * pd3dDevice)
{
	m_d3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
}
