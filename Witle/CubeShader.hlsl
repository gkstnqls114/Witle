
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


#include "Light.hlsl"

Texture2D gtxtAlbedoTexture : register(t6);
Texture2D gtxtSpecularTexture : register(t7);
Texture2D gtxtNormalTexture : register(t8);
Texture2D gtxtMetallicTexture : register(t9);
Texture2D gtxtEmissionTexture : register(t10);
Texture2D gtxtDetailAlbedoTexture : register(t11);
Texture2D gtxtDetailNormalTexture : register(t12);
// #define _WITH_VERTEX_LIGHTING

#define MAX_VERTEX_INFLUENCES			4
#define SKINNED_ANIMATION_BONES			128

cbuffer cbBoneOffsets : register(b7)
{
	float4x4 gpmtxBoneOffsets[SKINNED_ANIMATION_BONES];
};

cbuffer cbBoneTransforms : register(b8)
{
	float4x4 gpmtxBoneTransforms[SKINNED_ANIMATION_BONES];
};

struct VertexIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 normalW : NORMAL;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	//	nointerpolation float3 normalW : NORMAL;
	float4 cubeColor : COLOR0;
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR1;
#endif
};

VertexOut VS(VertexIn input)
{
	VertexOut output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxWorld);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	// output.uv = input.uv;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif
	output.cubeColor = input.color;
	return(output);

	//VertexOut vout;

	//vout.Pos = mul(mul(mul(float4(input.Pos, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	//vout.Color = input.Color; 
	//vout.normal = input.normal;

	//return vout;
}

float4 PS(VertexOut input) : SV_TARGET
{
	// float4 cColor = gtxtDiffuse.Sample(gSamplerState, input.uv);
	float4 cColor;
	if (testcolor.x == 0.f)
	{
		cColor = input.cubeColor;
	}
	else
	{
		cColor = float4(testcolor, 1.0f);
	}
#ifdef _WITH_VERTEX_LIGHTING
	float4 cIllumination = input.color;
#else
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
#endif
	return(cColor * cIllumination);


}


//VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
//{
//	VS_TEXTURED_LIGHTING_OUTPUT output;
//
//	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
//	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
//	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
//	output.uv = input.uv;
//#ifdef _WITH_VERTEX_LIGHTING
//	output.normalW = normalize(output.normalW);
//	output.color = Lighting(output.positionW, output.normalW);
//#endif
//	return(output);
//}
//
//float4 PSTexturedLighting(VS_TEXTURED_LIGHTING_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
//{
//	float4 cColor = gtxtDiffuse.Sample(gSamplerState, input.uv);
//#ifdef _WITH_VERTEX_LIGHTING
//	float4 cIllumination = input.color;
//#else
//	input.normalW = normalize(input.normalW);
//	float4 cIllumination = Lighting(input.positionW, input.normalW);
//#endif
//	return(cColor * cIllumination);
//}