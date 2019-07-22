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

//// ��Ʈ ���
//cbuffer cbHPpercentage : register(b5)
//{
//    float HPpercentage : packoffset(c0);
//}
// ���⼭�� 0~1�� ��Ÿ���� ���� �ۼ�������

//pickingPoint.x : ���� �ð�
//pickingPoint.y : �ִ� �ð�

VS_SCREEN_OUTPUT VSSkillIcon(VS_SCREEN_INPUT input, uint nVertexID : SV_VertexID)
{
    VS_SCREEN_OUTPUT output;
     
    // �ش� ��ũ�� ��ǥ�� ���� ��ǥ��� ��ȯ
    float ProjectionX = 2 * input.position.x / 1280.f - 1;
    float ProjectionY = -2 * input.position.y / 720.f + 1;
    output.position = float4(ProjectionX, ProjectionY, input.position.z, 1.f); 
      
    output.uv = input.uv; 

    return output;
}


float4 PSSkillIcon(VS_SCREEN_OUTPUT input) : SV_TARGET
{
    float4 pixColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    if (pixColor.a < 0.5f)
        discard;
     
    //find the angle between the centre and this pixel (between -180 and 180)
    // ������ ��׸��� ��ȯ... 
    float2 center_uv = float2(0.5, 0.5); 
    float angle = atan2(input.uv.y - center_uv.y, input.uv.x - center_uv.x) * 180.0 / 3.141592; //find the angle between the centre and this pixel (between -180 and 180)

    if (angle < 0)
    {
        angle += 360; //change angles to go from 0 to 360
    }
    
    if (angle <= (HPpercentage * 360.0))
    { 
        //if the angle is less than the progress angle blend the overlay colour
        pixColor -= float4(0.5, 0.5, 0.5, 0.5);
    } 

    
    return pixColor;
}