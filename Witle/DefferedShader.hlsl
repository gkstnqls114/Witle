#include "Variables.hlsl"
#include "Light.hlsl"


// gtxtAlbedoTexture : gbuffer 1
// gtxtSpecularTexture :  gbuffer 2;
// gtxtNormalTexture :  gbuffer 3;
// gtxtMetallicTexture :  gbuffer 4;

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
    float2(1.0, 1.0),
    float2(-1.0, -1.0),
    float2(1.0, -1.0)
};

struct VertexIn
{
    float4 position : POSITION;
    float2 uv : TEXCOORD; 
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOut VSMain(uint nVertexID : SV_VertexID)
{
    VertexOut output;
    
    //output.position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
    //output.uv = output.position.xy;
    
    if (nVertexID == 0)
    {
        output.position = float4(-1.0, 1.0, 0.5, 1.0);
        output.uv = float2(0.f, 0.f);
    }
    else if (nVertexID == 1)
    {
        output.position = float4(1.0, 1.0, 0.5, 1.0);
        output.uv = float2(1.f, 0.f);
    }
    else if (nVertexID == 2)
    {
        output.position = float4(-1.0, -1.0, 0.5, 1.0);
        output.uv = float2(0.f, 1.f);
    }
    else if (nVertexID == 3)
    {
        output.position = float4(-1.0, -1.0, 0.5, 1.0);
        output.uv = float2(0.f, 1.f);
    }
    else if (nVertexID == 4)
    {
        output.position = float4(1.0, 1.0, 0.5, 1.0);
        output.uv = float2(1.f, 0.f);
    }
    else if (nVertexID == 5)
    {
        output.position = float4(1.0, -1.0, 0.5, 1.0);
        output.uv = float2(1.f, 1.f);
    }

    return (output);
}

float4 PSMain(VertexOut input) : SV_TARGET
{
    float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
	 
//#ifdef _WITH_VERTEX_LIGHTING
//	float4 cIllumination = input.color;
//#else
//	input.normalW = normalize(input.normalW);
//	float4 cIllumination = Lighting(input.positionitionW, input.normalW);
//#endif
    return (cColor);

}


//VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
//{
//	VS_TEXTURED_LIGHTING_OUTPUT output;
//
//	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
//	output.positionitionW = (float3)mul(float4(input.positionition, 1.0f), gmtxGameObject);
//	output.positionition = mul(mul(float4(output.positionitionW, 1.0f), gmtxView), gmtxProjection);
//	output.uv = input.uv;
//#ifdef _WITH_VERTEX_LIGHTING
//	output.normalW = normalize(output.normalW);
//	output.color = Lighting(output.positionitionW, output.normalW);
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
//	float4 cIllumination = Lighting(input.positionitionW, input.normalW);
//#endif
//	return(cColor * cIllumination);
//}