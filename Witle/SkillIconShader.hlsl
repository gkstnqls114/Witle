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

//// 루트 상수
//cbuffer cbHPpercentage : register(b5)
//{
//    float HPpercentage : packoffset(c0);
//}
// 여기서는 0~1로 나타내는 남은 퍼센테이지

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

    return output;
}


float4 PSSkillIcon(VS_SCREEN_OUTPUT input) : SV_TARGET
{
    float4 pixColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    if (pixColor.a < 0.5f) discard;
      
    float2 center_uv = float2(0.5, 0.5);   
	float2 new_uv = float2(1 - input.uv.y - center_uv.y, 1- input.uv.x - center_uv.x);


    float angle = atan2(new_uv.y , new_uv.x) * 180.0 / 3.141592; // 라디안을 디그리로, - 180 ~ 180 으로 변경. 시계방향이 양의 방향임을 고려

	if (angle < 0)
	{
	    angle = angle + 360; // 0 ~ 360
	}
    
    
    if ((angle) <= (HPpercentage * 360.0))
    { 
        pixColor -= float4(0.5, 0.5, 0.5, 0.5);
    } 

    
    return pixColor;
}