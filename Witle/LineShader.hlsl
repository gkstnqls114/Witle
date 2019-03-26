
// ��Ʈ ���
cbuffer cbGameObjectInfo : register(b0)
{
	matrix gmtxWorld : packoffset(c0);
}

// ��� ����
cbuffer cbCameraInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
	float3 gvCameraPosition : packoffset(c8);
}

// ��Ʈ ���
cbuffer cbGameObjectInfo : register(b4)
{
	float3 testcolor: packoffset(c0);
}

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

float4 PS(VertexOut input) : SV_TARGET
{
	return input.color;
}