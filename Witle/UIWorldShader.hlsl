#include "Variables.hlsl"

struct VS_SCREEN_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VS_SCREEN_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};
 
const float HpGuage  =1.f;
const float MaxGuage = 100.f;

VS_SCREEN_OUTPUT VSUIWorld(VS_SCREEN_INPUT input)
{
    VS_SCREEN_OUTPUT output;
     
    float3 positionW = (float3) mul(float4(input.position, 1.0f), gmtxWorld);
    output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
    
    output.uv = input.uv * MaxGuage; 
     
    return output;
}


float4 PSUIWorld(VS_SCREEN_OUTPUT input) : SV_TARGET
{
    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    
    if (input.uv.x > HpGuage)
        color = float4(1.f, 1.f, 1.f, 1.f);

    return color;
}