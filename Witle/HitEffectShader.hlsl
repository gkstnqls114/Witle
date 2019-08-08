
#include "Variables.hlsl"

uniform float u_Number;

cbuffer cbPickingPoint : register(b4)
{
    float2 Resol : packoffset(c0);
}

// 루트 상수
cbuffer cbHPpercentage : register(b5)
{
    float ani_number : packoffset(c0);
}

// 여기서 cbPickingPoint 을 u_Resol_X, u_Resol_Y 로 사용하겠음
// 여기서 cbHPpercentage 을 u_Number 로 사용하겠음

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
 
VS_SCREEN_OUTPUT VSHitEffect(VS_SCREEN_INPUT input)
{
    VS_SCREEN_OUTPUT output;
     
    float3 positionW = (float3) mul(float4(input.position, 1.0f), gmtxWorld);
    output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
    output.uv = input.uv;
     
    return output;
}


float4 PSHitEffect(VS_SCREEN_OUTPUT input) : SV_TARGET
{ 
    float2 newTex;

    float fract_x = (ani_number / Resol.x) - floor(ani_number / Resol.x);

    float x_index = floor((fract_x) * Resol.x);
    float y_index = floor(floor(ani_number / Resol.y));
    
    newTex.x = input.uv.x / Resol.x + x_index / Resol.x;
    newTex.y = input.uv.y / Resol.y + y_index / Resol.y;
	 
    float4 color = gtxtTexture.Sample(gWrapSamplerState, newTex);
     
    if (color.a < 0.5)
        discard;

    return color;
}