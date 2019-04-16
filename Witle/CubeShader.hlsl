#include "Variables.hlsl"
#include "Light.hlsl"

// 루트 상수
cbuffer cbGameObjectInfo : register(b4)
{
	float3 testcolor: packoffset(c0);
}
 

struct VertexIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
    float2 uv : TEXCOORD; 
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
    output.uv = input.uv;
	return(output);

	//VertexOut vout;

	//vout.Pos = mul(mul(mul(float4(input.Pos, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	//vout.Color = input.Color; 
	//vout.normal = input.normal;

	//return vout;
}

float4 PS(VertexOut input) : SV_TARGET
{
    float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
	 
//#ifdef _WITH_VERTEX_LIGHTING
//	float4 cIllumination = input.color;
//#else
//	input.normalW = normalize(input.normalW);
//	float4 cIllumination = Lighting(input.positionW, input.normalW);
//#endif
	return(cColor ); 

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