#include "Variables.hlsl"
#include "Light.hlsl"


struct VertexIn
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float2 time : TIME; // LIFE Time, EndTime 생명주기
    float3 velocity : VELOCITY; // LIFE Time, EndTime 생명주기
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION; 
    float2 uv : TEXCOORD;
	//	nointerpolation float3 normalW : NORMAL; 
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR1;
#endif
};

VertexOut VS(VertexIn input)
{
    VertexOut output;

    output.positionW = (float3) mul(float4(input.position, 1.0f), gmtxWorld);
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.uv = input.uv;

	// output.uv = input.uv; 
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif 
	//return vout;
    float LifeTime = input.time.x;
    float StartTime = input.time.y;
    float3 newPos = output.positionW;
	
    // HP_percentage를 시ㅏㄱㄴ으ㅗㄹ 대체
    float newTime = HPpercentage - StartTime; //소수점 부분만 존재하도록 떼어낸다. 즉 1.2 인경우 0.2
	
    if (newTime > 0)
    {
        float life = newTime;

        float remainingLife = LifeTime - life;
         
        remainingLife = 1.f;
        newTime = fmod(newTime, LifeTime);
        

        if (remainingLife > 0)
        {
            newPos.x += ((input.velocity.x * newTime) /*+ (0.5 * c_Gravity.x * newTime * newTime)*/);
            newPos.y += ((input.velocity.y * newTime) /*+ (0.5 * c_Gravity.y * newTime * newTime)*/);
            newPos.z += ((input.velocity.z * newTime) /*+ (0.5 * c_Gravity.z * newTime * newTime)*/);
        }
        else
        {
            newPos = float3(10000, 10000, 10000);
        }
    }
    else
    {
        newPos = float3(10000, 10000, 10000);
    }
	 
    output.position = float4(1.F, 1.F, 1.F, 1.F); /*vec4(newPos, 1)*/;
    
    return (output);
}

float4 PS(VertexOut input) : SV_TARGET
{
    float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
	 
//#ifdef _WITH_VERTEX_LIGHTING
//	float4 cIllumination = input.color;
//#else
//	input.normalW = normalize(input.normalW);
//	float4 cIllumination = Lighting(input.positionW, input.normalW);
//#endif
    return (cColor);

}


//VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
//{
//	VS_TEXTURED_LIGHTING_OUTPUT output;
//
//	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
//	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
//	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
//	output.uv = input.uv;
//#ifdef _WITH_VERTEX_LIGHTING
//	output.normalW = normalize(output.normalW);
//	output.color = Lighting(output.positionW, output.normalW);
//#endif
//	return(output);
//}
//
//float4 PSTexturedLighting(VS_TEXTURED_LIGHTING_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
//{
//	float4 cColor = gtxtDiffuse.Sample(gSamplerState, input.uv);
//#ifdef _WITH_VERTEX_LIGHTING
//	float4 cIllumination = input.color;
//#else
//	input.normalW = normalize(input.normalW);
//	float4 cIllumination = Lighting(input.positionW, input.normalW);
//#endif
//	return(cColor * cIllumination);
//}