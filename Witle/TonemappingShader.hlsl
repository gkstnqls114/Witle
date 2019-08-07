 #include "Variables.hlsl"
 
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// 정점 셰이더를 정의한다.
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


// texturebase : bloom 위한 텍스쳐
float3 ToneMapping(float3 HDRColor)
{
    // 현재 픽셀에 대한 휘도 스케일 계산
    float LScale = dot(HDRColor, LUM_FACTOR);
    LScale = float(MiddleGrey / gAverageLum[0]) * LScale;
    LScale = (LScale + (LScale * LScale / LumWhiteSqr)) / (1.0 + LScale);

    // 휘도 스케일을 픽셀 색상에 적용
    return HDRColor * LScale;
}

// texturebase : bloom 위한 텍스쳐
float3 ToneMappingAndBloom(PS_INPUT input)
{
    float3 Color = gtxtTexture.Sample(gssClamp, input.uv).xyz;

    // Bloom 분포 추가
    Color += fBloomScale * gtxtTerrainBaseTexture.Sample(gssClamp, input.uv).xyz;

    return Color;
    //// 현재 픽셀에 대한 휘도 스케일 계산
    //float LScale = dot(Color, LUM_FACTOR);
    //LScale = float(MiddleGrey / gAverageLum[0]) * LScale;
    //LScale = (LScale + (LScale * LScale / LumWhiteSqr)) / (1.0 + LScale);

    //// 휘도 스케일을 픽셀 색상에 적용
    //return Color * LScale;
}

//픽셀 셰이더를 정의한다.
float4 PSMain(PS_INPUT input) : SV_TARGET
{
    //float3 cColor = gtxtTexture.Sample(gssClamp, input.uv).xyz;
    //cColor = ToneMapping(cColor);

    float3 cColor = ToneMappingAndBloom(input);
    
    return float4(cColor, 1.0);
}
 