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

static matrix Identity =
{
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
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
    //  gmtxInstancingWorld[nInstanceID].m_mtxWorld에서 SCALE 행렬도 사용하는 듯
    matrix newWorld = Identity; 
    newWorld._41 = gmtxInstancingWorld[nInstanceID].m_mtxWorld._41;
    newWorld._43 = gmtxInstancingWorld[nInstanceID].m_mtxWorld._43;

    float3 positionW = mul(float4(input.position, 1.0f), newWorld).xyz;
    
    output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
    output.color = input.color;
    return (output);
}

float4 PS(VertexOut input) : SV_TARGET
{
    return float4(1, 0,0,1);
}