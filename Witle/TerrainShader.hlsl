#include "Variables.hlsl"

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1; 
    float fogFactor : FOG;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

// #ifdef _WITH_CONSTANT_BUFFER_SYNTAX
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
//#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
//#endif
	output.color = input.color;
	output.uv0 = input.uv0 * 20;
	output.uv1 = input.uv1;

    float4 positionW = mul(float4(input.position, 1.0f), gmtxWorld);
    float Z = length(gvCameraPosition - float3(positionW.xyz));
    
    if (Z > 20000)
        Z = 0;
    float fogEnd = 10000;
    float fogStart = 500;
    output.fogFactor = saturate((fogEnd - Z) / (fogEnd - fogStart)); 

	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gWrapSamplerState, input.uv0);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gWrapSamplerState, input.uv1);
	float4 cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
    
    cColor = 1.4 * cColor;
    float4 fogColor = float4(0.0 / 255.0, 34.0 / 255.0, 102.0 / 255.0, 1.0f);
    float4 finalColor = input.fogFactor * cColor + (1.0 - input.fogFactor) * fogColor;

    return (finalColor);
}

PS_OUTPUT_FOR_GBUFFERS PSTerrainForGBuffers(VS_TERRAIN_OUTPUT input)
{
    PS_OUTPUT_FOR_GBUFFERS output;
       
    float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gWrapSamplerState, input.uv0);
    float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gWrapSamplerState, input.uv1);
    float4 cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

    // gbuffer ����ü�� ��ŷ
    float SpecPowerNorm = NormalizeSpecPower(1); // ����ŧ�� �Ŀ� ����ȭ
    float SpecIntensity = 1; 
    output.ColorSpecInt = float4(cColor.rgb, SpecIntensity);
    output.Normal = float4(float3(0.f, 1.f, 0.f) * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(SpecPowerNorm, 0, 0, 0);

    return (output);
} 