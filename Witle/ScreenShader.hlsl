#include "Variables.hlsl"

struct VS_SCREEN_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0; 
};

struct VS_SCREEN_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};
 
VS_SCREEN_OUTPUT VSScreen(VS_SCREEN_INPUT input, uint nVertexID : SV_VertexID)
{
    VS_SCREEN_OUTPUT output;
     
    // 해당 스크린 좌표를 투영 좌표계로 변환
    float ProjectionX = 2 * input.position.x / 1280.f - 1;
    float ProjectionY = -2 * input.position.y / 720.f + 1;
     
    output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f);
    output.uv = input.uv * 100.f; // hp 퍼센트 위해...
    output.color = float4(1.f, 1.f, 1.f, 1.f);
     
    return output;
}


float4 PSScreen(VS_SCREEN_OUTPUT input) : SV_TARGET
{ 
    float4 color = gtxtTexture.Sample(gssClamp, input.uv);
    if (color.a < 0.5) discard;

    if (input.uv.x > HPpercentage)
        color = float4(0.f, 0.f, 0.f, 1.f); 

    return color;
}