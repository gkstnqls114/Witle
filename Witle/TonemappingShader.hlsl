 #include "Variables.hlsl"
 
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

    return (output);
}

const float MiddleGrey = -100.f;
const float LumWhiteSqr = 0.f;

float3 ToneMapping(float3 HDRColor)
{
    // ���� �ȼ��� ���� �ֵ� ������ ���
    float LScale = dot(HDRColor, LUM_FACTOR);
    LScale = LScale * (MiddleGrey / gAverageLum[0]);
    //LScale = (LScale + LScale * LScale / LumWhiteSqr) / (1.0 + LScale);

    // �ֵ� �������� �ȼ� ���� ����
    return HDRColor * LScale;
}

//�ȼ� ���̴��� �����Ѵ�.
float4 PSMain(PS_INPUT input) : SV_TARGET
{
    float3 cColor = gtxtTerrainBaseTexture.Sample(gssClamp, input.uv).xyz;
    
    cColor = ToneMapping(cColor);

    return float4(cColor, 1.f);
}
 