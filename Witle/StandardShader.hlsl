
// 루트 상수
cbuffer cbGameObjectInfo : register(b0)
{
	matrix gmtxWorld : packoffset(c0);
}

// 상수 버퍼
cbuffer cbCameraInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
	float3 gvCameraPosition : packoffset(c8);
}

// 루트 상수
cbuffer cbGameObjectInfo : register(b4)
{
	float3 testcolor: packoffset(c0);
}

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

Texture2D gtxtAlbedoTexture : register(t6);
Texture2D gtxtSpecularTexture : register(t7);
Texture2D gtxtNormalTexture : register(t8);
Texture2D gtxtMetallicTexture : register(t9);
Texture2D gtxtEmissionTexture : register(t10);
Texture2D gtxtDetailAlbedoTexture : register(t11);
Texture2D gtxtDetailNormalTexture : register(t12);

SamplerState gssWrap : register(s0);

#include "Light.hlsl"

//cbuffer cbCameraInfo : register(b1)
//{
//	matrix					gmtxView : packoffset(c0);
//	matrix					gmtxProjection : packoffset(c4);
//	float3					gvCameraPosition : packoffset(c8);
//};
//
//cbuffer cbGameObjectInfo : register(b2)
//{
//	matrix					gmtxGameObject : packoffset(c0);
//};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//struct VS_WIREFRAME_INPUT
//{
//	float3 position : POSITION;
//};
//
//struct VS_WIREFRAME_OUTPUT
//{
//	float4 position : SV_POSITION;
//};
//
//VS_WIREFRAME_OUTPUT VSWireFrame(VS_WIREFRAME_INPUT input)
//{
//	VS_WIREFRAME_OUTPUT output;
//
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
//
//	return(output);
//}
//
//float4 PSWireFrame(VS_WIREFRAME_OUTPUT input) : SV_TARGET
//{
//	return(float4(0.0f, 0.0f, 1.0f, 1.0f));
//}

struct VS_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_STANDARD_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
};

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
	VS_STANDARD_OUTPUT output;

	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.tangentW = mul(input.tangent, (float3x3)gmtxWorld);
	output.bitangentW = mul(input.bitangent, (float3x3)gmtxWorld);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSStandard(VS_STANDARD_OUTPUT input) : SV_TARGET
{
	return float4(1.f, 0.f, 0.f, 1.f);
	// 임시로 사용할 컬러 색깔
	float TESTColor = float4(1.f, 1.f, 1.f, 1.f);
	//float4 cAlbedoColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_ALBEDO_MAP) cAlbedoColor = gtxtAlbedoTexture.Sample(gssWrap, input.uv);
	//float4 cSpecularColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_SPECULAR_MAP) cSpecularColor = gtxtSpecularTexture.Sample(gssWrap, input.uv);
	//float4 cNormalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_NORMAL_MAP) cNormalColor = gtxtNormalTexture.Sample(gssWrap, input.uv);
	//float4 cMetallicColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_METALLIC_MAP) cMetallicColor = gtxtMetallicTexture.Sample(gssWrap, input.uv);
	//float4 cEmissionColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_EMISSION_MAP) cEmissionColor = gtxtEmissionTexture.Sample(gssWrap, input.uv);

	float3 normalW;
	//float4 cColor = cAlbedoColor + cSpecularColor + cMetallicColor + cEmissionColor;
	//if (gnTexturesMask & MATERIAL_NORMAL_MAP)
	//{
	//	float3x3 TBN = float3x3(normalize(input.tangentW), normalize(input.bitangentW), normalize(input.normalW));
	//	float3 vNormal = normalize(cNormalColor.rgb * 2.0f - 1.0f); //[0, 1] → [-1, 1]
	//	normalW = normalize(mul(vNormal, TBN));
	//}
	//else
	//{
		normalW = normalize(input.normalW);
	//}
	float4 cIllumination = Lighting(input.positionW, normalW);
	return(lerp(TESTColor, cIllumination, 0.5f));
}