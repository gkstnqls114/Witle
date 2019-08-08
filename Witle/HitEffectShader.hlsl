
#include "Variables.hlsl"

// ��Ʈ ���
cbuffer cbresol : register(b4)
{
    float2 resol : packoffset(c0);
}

// ��Ʈ ���
cbuffer cbindex : register(b5)
{
    float index : packoffset(c0);
}

uniform float u_Number;
 
// ���⼭ cbresol �� u_resol_X, u_resol_Y �� ����ϰ���
// ���⼭ cbindex �� u_Number �� ����ϰ���

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
     
    float fract_x = (index / resol.x) - floor(index / resol.x);

    float x_index = floor((fract_x) * resol.x);
    float y_index = floor(floor(index / resol.y));
     
    float2 newTex;
    newTex.x = input.uv.x / resol.x + x_index / resol.x;
    newTex.y = input.uv.y / resol.y + y_index / resol.y;
	  
    output.uv = newTex;
     
    return output;
}


float4 PSHitEffect(VS_SCREEN_OUTPUT input) : SV_TARGET
{ 
    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv);
     
    //if (color.a < 0.5)
    //    discard;

    return color;
}