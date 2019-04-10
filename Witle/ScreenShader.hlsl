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

const int2 ClientSize = int2(800, 600);

VS_SCREEN_OUTPUT VSScreen(VS_SCREEN_INPUT input, uint nVertexID : SV_VertexID)
{
    VS_SCREEN_OUTPUT output;

    output.color = float4(1.f, 1.f, 1.f, 1.f);
   
    //// 해당 스크린 좌표를 투영 좌표계로 변환
    //float ProjectionX = 2 / ClientSize.x * (input.position.x - (ClientSize.x / 2));
    //float ProjectionY = -2 / ClientSize.y * (input.position.y - (ClientSize.y / 2));
   
    //output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f);
    //output.uv = input.uv;
 
    float2 lefttop =     float2(-0.5, 0.5);
    float2 righttop =    float2(0.5, 0.5);
    float2 leftbottom =  float2(-0.5, -0.5);
    float2 rightbottom = float2(0.5, -0.5);

    if (nVertexID == 0)
    {
        output.position = float4(lefttop, 0.5, 1.0);
        output.uv = float2(0.f, 0.f);
    }
    else if (nVertexID == 1)
    {
        output.position = float4(righttop, 0.5, 1.0);
        output.uv = float2(1.f, 0.f);
    }
    else if (nVertexID == 2)
    {
        output.position = float4(leftbottom, 0.5, 1.0);
        output.uv = float2(0.f, 1.f);
    }
    else if (nVertexID == 3)
    {
        output.position = float4(leftbottom, 0.5, 1.0);
        output.uv = float2(0.f, 1.f);
    }
    else if (nVertexID == 4)
    {
        output.position = float4(righttop, 0.5, 1.0);
        output.uv = float2(1.f, 0.f);
    }
    else if (nVertexID == 5)
    {
        output.position = float4(rightbottom, 0.5, 1.0);
        output.uv = float2(1.f, 1.f);
    }

    return output;
}


float4 PSScreen(VS_SCREEN_OUTPUT input) : SV_TARGET
{
  //  float4 color = input.color;
  //  float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv0);
    return float4(1.f, 1.f, 1.f, 1.f);
}