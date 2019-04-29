#include "Variables.hlsl"

struct VertexIn
{
	float3 position : POSITION;
	float4 color : COLOR; 
};

struct VertexOut
{
	float4 position : SV_POSITION; 
	float4 color : COLOR;
};

VertexOut VS(VertexIn input)
{
	VertexOut output;
	 
	float3 positionW = (float3)mul(float4(input.position, 1.0f), gmtxWorld);

	output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
	output.color = input.color;
	return(output); 
}

VertexOut VSInstancing(VertexIn input, uint nInstanceID : SV_InstanceID)
{ 
    VertexOut output;
    float3  positionW = mul(float4(input.position, 1.0f), gmtxInstancingWorld[nInstanceID].m_mtxWorld).xyz;
    
    output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
    output.color = input.color;
    return (output);
}

float4 PS(VertexOut input) : SV_TARGET
{
    return float4(1, 0,0,1);
}