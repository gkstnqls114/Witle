#include "Variables.hlsl"

struct VS_SCREEN_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv0 : TEXCOORD0; 
};

struct VS_SCREEN_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv0 : TEXCOORD0;
};

VS_SCREEN_OUTPUT VSScreen(VS_SCREEN_INPUT input)
{
    VS_SCREEN_OUTPUT output;

    output.position = float4(1.f, 1.f, 1.f, 1.f);
    output.color = float4(1.f, 1.f, 1.f, 1.f);
    output.uv0 = float2(1.f, 1.f);

    // 해당 스크린 좌표를 투영 좌표계로 변환
    return output;
}


float4 PSTerrain(VS_SCREEN_OUTPUT input) : SV_TARGET
{
    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv0);

    return color;
}