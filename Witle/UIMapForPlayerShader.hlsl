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

// pickingPoint를 플레이어 위치로 삼는다.
 
VS_SCREEN_OUTPUT VSUIMapForPlayer(VS_SCREEN_INPUT input, uint nVertexID : SV_VertexID)
{
    VS_SCREEN_OUTPUT output;
     
    // 해당 스크린 좌표를 투영 좌표계로 변환
    float newX = input.position.x + pickingPoint.x;
    float newY = input.position.y + pickingPoint.y;

    float ProjectionX = 2 * newX / 1280.f - 1;
    float ProjectionY = -2 * newY / 720.f + 1;
    
    output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f);
    output.uv = input.uv;
     
    return output;
}


float4 PSUIMapForPlayer(VS_SCREEN_OUTPUT input) : SV_TARGET
{
    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    return color;
}