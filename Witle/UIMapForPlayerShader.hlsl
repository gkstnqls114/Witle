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

// pickingPoint�� �÷��̾� ��ġ�� ��´�.
 
VS_SCREEN_OUTPUT VSUIMapForPlayer(VS_SCREEN_INPUT input, uint nVertexID : SV_VertexID)
{
    VS_SCREEN_OUTPUT output;
     
    // �ش� ��ũ�� ��ǥ�� ���� ��ǥ��� ��ȯ
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