
// 루트 상수
cbuffer cbGameObjectInfo : register(b0)
{
	matrix gmtxWorld : packoffset(c0);
}

// 상수 버퍼
cbuffer cbGameObjectInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
}

struct VertexIn
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn input)
{
	VertexOut vout;

	//vout.Pos = mul(float4(input.Pos, 1.0f), gmtxWorld);
	vout.Pos = mul(mul(mul(float4(input.Pos, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	vout.Color = input.Color; 

	return vout;
}

float4 PS(VertexOut input) : SV_TARGET
{
	return input.Color;
}
