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
     
    // �ش� ��ũ�� ��ǥ�� ���� ��ǥ��� ��ȯ
    float ProjectionX = 2 * input.position.x / 1280.f - 1;
    float ProjectionY = -2 * input.position.y / 720.f + 1;
     
    output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f);
    output.uv = input.uv;
    output.color = float4(1.f, 1.f, 1.f, 1.f);
     
    return output;
}


float4 PSScreen(VS_SCREEN_OUTPUT input) : SV_TARGET
{
  //  float4 color = input.color; 
    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    if (color.a < 0.5) discard;
    return color;
}