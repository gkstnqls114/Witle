
SamplerState gWrapSamplerState : register(s0);

Texture2D gtxtTexture : register(t0);
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

// ���� ���̴��� �����Ѵ�.
PS_INPUT VSMain(uint nVertexID : SV_VertexID)
{
	PS_INPUT output;
	
if (nVertexID == 0)
{
	output.pos = float4(-1.0, 1.0, 0.5, 1.0);
	output.uv = float2(0.f, 0.f);
}
else if (nVertexID == 1)
{
	output.pos = float4(1.0, 1.0, 0.5, 1.0);
	output.uv = float2(1.f, 0.f);
}
else if (nVertexID == 2)
{
	output.pos = float4(-1.0, -1.0, 0.5, 1.0);
	output.uv = float2(0.f, 1.f);
}
else if (nVertexID == 3)
{
	output.pos = float4(-1.0, -1.0, 0.5, 1.0);
	output.uv = float2(0.f, 1.f);
}
else if (nVertexID == 4)
{
	output.pos = float4(1.0, 1.0, 0.5, 1.0);
	output.uv = float2(1.f, 0.f);
}
else if (nVertexID == 5)
{
	output.pos = float4(1.0, -1.0, 0.5, 1.0);
	output.uv = float2(1.f, 1.f);
}

	return(output);
}

//�ȼ� ���̴��� �����Ѵ�.
float4 PSMain(PS_INPUT input) : SV_TARGET
{ 
	float4 cColor = gtxtTerrainBaseTexture.Sample(gWrapSamplerState, input.uv);
	return(cColor);
}
 