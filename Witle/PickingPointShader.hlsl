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
     
    output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f);
    
    output.uv = input.uv;
    output.color = float4(1.f, 1.f, 1.f, 1.f);
     
    return output;
}


float4 PSScreen(VS_SCREEN_OUTPUT input) : SV_TARGET
{
  //  float4 color = input.color; 
    // compute sin and cos for this angle 
    //float time = (totalTime);
    //matrix rotate =
    //{
    //    1, 0, 0, 0,
    //    0, cos(time), sin(time), 0,
    //    0, -sin(time), cos(time), 0,
    //    0, 0, 0, 1
    //};
    //float s = sin(60.f * 180.0f * 3.141592f);
    //float c = cos(60.f * 180.0f * 3.141592f); 
    
    //float2 center = float2(1280/4, 720/4);

    //// move the rotation center to the origin : 중점이동 (center는 기초값을 0.5로 하면 중심이 되것지)
    //float2 uv = input.uv - center;
  
    // // rotate the uv : 기본 UV 좌표와의 dot연산 
    //float2 rotateduv;
    //rotateduv.x = dot(uv, float2(s, -c));
    //rotateduv.y = dot(uv, float2(s, c));
  
    //// move the uv's back to the correct place 
    //rotateduv += center;
  
    //float4 color = gtxtTexture.Sample(gWrapSamplerState, rotateduv);
    //if (color.a < 0.5) discard;

    //return color;
    
    //float4 uv4 = mul(float4(input.uv, 0.f, 0.f), rotate);
    //float2 newUV = float2(fmod(uv4.x, 1), fmod(uv4.y, 1));

    float4 color = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    if (color.a < 0.5)
        discard;
    return color;
}