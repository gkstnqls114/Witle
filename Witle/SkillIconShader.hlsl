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
    float percentage : TIME;
};
 

//pickingPoint.x : 남은 시간
//pickingPoint.y : 최대 시간

VS_SCREEN_OUTPUT VSSkillIcon(VS_SCREEN_INPUT input, uint nVertexID : SV_VertexID)
{
    VS_SCREEN_OUTPUT output;
     
    // 해당 스크린 좌표를 투영 좌표계로 변환
    float ProjectionX = 2 * input.position.x / 1280.f - 1;
    float ProjectionY = -2 * input.position.y / 720.f + 1;
    
    output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f);
    output.uv = input.uv; 
    output.percentage = (pickingPoint.y - pickingPoint.x) / pickingPoint.y;
     
    return output;
}


float4 PSSkillIcon(VS_SCREEN_OUTPUT input) : SV_TARGET
{
    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    if (color.a < 0.5f)
        discard;
    
    return color;
}