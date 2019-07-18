#include "Variables.hlsl"
#include "Light.hlsl"
 
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
    float4 positionW : POSITION0;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1; 
    float4 shadowPosition : POSITION1;
    float fogFactor : FOG;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

// #ifdef _WITH_CONSTANT_BUFFER_SYNTAX
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
//#else
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld);
    output.position = mul(mul(output.positionW, gmtxView), gmtxProjection);
//#endif
//#endif
	output.color = input.color;
	output.uv0 = input.uv0 * 20;
	output.uv1 = input.uv1;
     
    // 그림자 위해 설정 /////////// 
    output.shadowPosition = mul(output.positionW, gShadowTransform);
    // 그림자 위해 설정 ///////////
     
    float Z = length(gvCameraPosition - output.positionW.xyz);
    
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
    
    cColor = 1.2 * cColor;
    float4 fogColor = float4(0.0 / 255.0, 34.0 / 255.0, 102.0 / 255.0, 1.0f);
    float4 finalColor = input.fogFactor * cColor + (1.0 - input.fogFactor) * fogColor;
      
    //// 그림자 계산 1 ...
    //input.shadowPosition = input.shadowPosition / input.shadowPosition.w;
     
    //float fShadowFactor = 0.0;
    //float fBias = 0.006f; 
    //float fsDepth = gtxtShadow.SampleCmpLevelZero(gssPCFSampler, input.shadowPosition.xy, input.shadowPosition.z).r;
    //// float fsDepth = gtxtShadow.Sample(gssClamp, input.shadowPosition.xy).r;

    //if (input.shadowPosition.z <= (fsDepth + fBias))
    //    fShadowFactor = 1.f; // 그림자가 아님
    //// 그림자 계산 1 ...
     
    float3 fShadowFactor = float3(1.0f, 1.0f, 1.0f);
    fShadowFactor[0] = CalcShadowFactor(input.shadowPosition);
    
    // 현재 바닥이 그냥 위를 바라보고 있으므로...
    float3 normalW = float3(0.0, 1.0, 0.0);

    float4 cllumination = Lighting(input.positionW.xyz, normalW, fShadowFactor[0]);
     
    finalColor = lerp(finalColor, cllumination, 0.5f);
    
    return (finalColor);
}

PS_OUTPUT_FOR_GBUFFERS PSTerrainForGBuffers(VS_TERRAIN_OUTPUT input)
{
    PS_OUTPUT_FOR_GBUFFERS output;
       
    float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gWrapSamplerState, input.uv0);
    float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gWrapSamplerState, input.uv1);
    float4 cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

    // gbuffer 구조체에 패킹
    float depth = input.position.z / input.position.w;
    if (depth > 0.01f)
        depth = 1.f;
    float SpecPowerNorm = NormalizeSpecPower(1); // 스페큘러 파워 정규화
    float SpecIntensity = 1; 
    output.ColorSpecInt = float4(cColor.rgb, SpecIntensity);
    output.Normal = float4(float3(0.f, 1.f, 0.f) * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(depth, depth, depth, 0);

    return (output);
} 