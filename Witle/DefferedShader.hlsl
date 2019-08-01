#include "Variables.hlsl"
#include "Light.hlsl"

// gtxtAlbedoTexture : gbuffer 0 ( base color + specint)
// gtxtSpecularTexture :  gbuffer 1 (nomral);
// gtxtNormalTexture :  gbuffer 2; (어디다가 쓰는거엿지)
// gtxtMetallicTexture :  gbuffer 4 (깊이);

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
    float2(1.0, 1.0),
    float2(-1.0, -1.0),
    float2(1.0, -1.0)
};
 
struct VertexOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD; 
    //float3 positionW : POSITION;
    //float4 shadowPosition : TEXCOORD1;
};

struct SURFACE_DATA
{
    float LinearDepth;
    float3 Color;
    float3 Noraml;
    float SpecInt;
    float SpecPow;
};

float4 GetPerspectiveValuses()
{
    float4 output; // x, y 는 투영행렬의 대각선 방향값, z는 [3][2], w는 [2][2]
    output.x = 1.0 / gmtxProjection[0][0];
    output.y = 1.0 / gmtxProjection[1][1];
    output.z = gmtxProjection[3][2];
    output.w = -gmtxProjection[2][2];
    return output;
}

float ConvertDepthToLinear(float depth)
{
    float4 PerspectiveValuses = GetPerspectiveValuses();

    float linearDepth = PerspectiveValuses.z / (depth + PerspectiveValuses.w);
    return linearDepth;
}

SURFACE_DATA UnpackGBuffers(int2 location)
{
    SURFACE_DATA output;

    // 로드함수 위한 3컴포넌트 선언
    int3 location3 = int3(location, 0);

    // 깊이 값 추출 및 선형 깊이 값으로 변환
    float depth = gtxtMetallicTexture.Load(location3, 0);
    output.LinearDepth = ConvertDepthToLinear(depth);

    // 베이스 색상과 스페큘러 세기 값 추출
    float4 baseColorSpecInt = gtxtAlbedoTexture.Load(location3);
    output.Color = baseColorSpecInt.xyz;
    output.SpecInt = baseColorSpecInt.w;

    // 노멀 샘플링 후 전체 범위 변환 및 정규화
    output.Noraml = gtxtSpecularTexture.Load(location3);
    output.Noraml = normalize(output.Noraml * 2.0 - 1.0);

    // 원래 범위 값에 대해 스페큘러 파워 스케일 조정
    // 안쓸거같아서 책에 나온 코드랑 다르당
    float specPowerNorm = gtxtNormalTexture.Load(location3).x;
    output.SpecPow = specPowerNorm;
     
    return output;

}

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

float3 CalcWorldPos(float2 csPos, float linearDepth)
{
    float4 pos;
    pos.xy = csPos.xy * GetPerspectiveValuses().xy * linearDepth;
    pos.z = linearDepth;
    pos.w = 1.0;
    return pos;
}

float4 PSMain(VertexOut input) : SV_TARGET
{
    // gbuffer 언패킹
    SURFACE_DATA gdb = UnpackGBuffers(input.position.xy); 

    // 데이터 변환
    float3 normalW = gdb.Noraml;

    float4 diffuseColor;
    diffuseColor.xyz = gdb.Color;
    diffuseColor.w = 1.0f; 
     
    float3 posW = CalcWorldPos(input.uv, gdb.LinearDepth);
    
    float4 shadowPosition = mul(float4(posW, 1.f), gShadowTransform);
    float fShadowFactor = CalcShadowFactor(shadowPosition);
     
    float4 cIllumination = Lighting(posW, normalW, fShadowFactor);
    
    float fogEnd = 30000;
    float fogStart = 50; 
    float distance = sqrt(posW.z * posW.z + posW.x * posW.x); // 아니 왜 pos.z 하니까 되는거냐
    
    float fogFactor = (saturate((fogEnd - distance) / (fogEnd - fogStart))); 
    
    float4 fogColor = float4(0.0 / 255.0, 34.0 / 255.0, 102.0 / 255.0, 1.0f);
      
    return ((fogFactor) * lerp(diffuseColor, cIllumination, 0.5f) + (1.0 - fogFactor) * fogColor);
}
 