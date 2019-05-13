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
    float ProjectionX = 2 * (input.position.x + pickingPoint.x) / 1280.f - 1;
    float ProjectionY = -2 * (input.position.y + pickingPoint.y) / 720.f + 1;
     
    //float time = (totalTime);
    //matrix rotate =
    //{
    //    0.5, 0, 0, 0,
    //    0, 0.5, 0, 0,
    //    0, 0, 0.5 0, 0,
    //    0, 0, 0, 0.5
    //};
    //matrix rotate =
    //{
    //    1, 0, 0, 0,
    //    0, 1, 0, 0,
    //    0, 0, 1, 0,
    //    0, 0, 0, 1
    //};
    output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f);
    // output.position = mul(output.position, rotate);

    output.uv = input.uv;
    output.color = float4(1.f, 1.f, 1.f, 1.f);
     
    return output;
}


float4 PSScreen(VS_SCREEN_OUTPUT input) : SV_TARGET
{ 
    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    if (color.a < 0.5) discard;
    
    return color;
}