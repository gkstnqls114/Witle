
// 루트 상수
cbuffer cbGameObjectInfo : register(b0)
{
	matrix gmtxWorld : packoffset(c0);
}

// 상수 버퍼
cbuffer cbCameraInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
	float fPosition : packoffset(c8);
}

// 루트 상수
cbuffer cbGameObjectInfo : register(b4)
{
	float3 color: packoffset(c0);
}

struct VertexIn
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float3 normal : NORMAL;
};

struct VertexOut
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float3 normal : NORMAL;
};

VertexOut VS(VertexIn input)
{
	VertexOut vout;

	// vout.Pos = mul(float4(input.Pos, 1.0f), gmtxWorld);
	vout.Pos = mul(mul(mul(float4(input.Pos, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	vout.Color = input.Color; 
	vout.normal = input.normal;

	return vout;
}

float4 PS(VertexOut input) : SV_TARGET
{
	if (color.x == 0.f)
	{
		return input.Color;
	}
	else
	{
		return float4(color, 1.0f);
	}
}
