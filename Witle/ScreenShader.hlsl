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

const int2 ClientSize = int2(800, 600);

VS_SCREEN_OUTPUT VSScreen(VS_SCREEN_INPUT input)
{
    VS_SCREEN_OUTPUT output;

    output.position = float4(1.f, 1.f, 1.f, 1.f);
    output.color = float4(1.f, 1.f, 1.f, 1.f);
    output.uv0 = float2(1.f, 1.f);
     
    // �ش� ��ũ�� ��ǥ�� ���� ��ǥ��� ��ȯ
    float ProjectionX = 2 / ClientSize.x * (input.position.x - (ClientSize.x / 2));
    float ProjectionY = -2 / ClientSize.y * (input.position.y - (ClientSize.y / 2));

    output.position = float4(ProjectionX, ProjectionY, 1.f, 1.f);
    output.uv0 = input.uv0;
     
    return output;
}


float4 PSScreen(VS_SCREEN_OUTPUT input) : SV_TARGET
{
    float4 color = input.color;
  //  float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv0);
    return color;
}