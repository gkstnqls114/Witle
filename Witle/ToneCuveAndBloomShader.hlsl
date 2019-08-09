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
float3 ToneCurveAndBloom(PS_INPUT input)
{
    float3 Color = gtxtTexture.Sample(gssClamp, input.uv).xyz;

    // Bloom 분포 추가
    Color += fBloomScale * gtxtTerrainBaseTexture.Sample(gssClamp, input.uv).xyz;

    Color.r = quadraticBezier(Color.r, 0.48, 0.26); 
    Color.g = quadraticBezier(Color.g, 0.5, 0.25);
    Color.b = quadraticBezier(Color.b, 0.5, 0.28);
    
    return Color; 
}

 // texturebase : bloom 위한 텍스쳐
float3 Bloom(PS_INPUT input)
{
    float3 Color = gtxtTexture.Sample(gssClamp, input.uv).xyz;

    // Bloom 분포 추가
    Color += fBloomScale * gtxtTerrainBaseTexture.Sample(gssClamp, input.uv).xyz;
     
    return Color;
}

// texturebase : bloom 위한 텍스쳐
float3 ToneCurve(PS_INPUT input)
{
    float3 Color = gtxtTexture.Sample(gssClamp, input.uv).xyz;
     
    Color.r = quadraticBezier(Color.r, 0.5, 0.25);
    Color.g = quadraticBezier(Color.g, 0.5, 0.25);
    Color.b = quadraticBezier(Color.b, 0.5, 0.25);

    return Color; 
}

//픽셀 셰이더를 정의한다.
float4 PSToneCurveAndBloom(PS_INPUT input) : SV_TARGET
{ 
    float3 cColor = ToneCurveAndBloom(input);
    
    return float4(cColor, 1.0);
}
 

//픽셀 셰이더를 정의한다.
float4 PSBloom(PS_INPUT input) : SV_TARGET
{
    float3 cColor = Bloom(input);
    
    return float4(cColor, 1.0);
}
 

//픽셀 셰이더를 정의한다.
float4 PSToneCurve(PS_INPUT input) : SV_TARGET
{
    float3 cColor = ToneCurve(input);
    
    return float4(cColor, 1.0);
}