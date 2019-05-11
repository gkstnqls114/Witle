#include "Variables.hlsl"
#include "Light.hlsl"
 
struct VS_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_STANDARD_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
    float fogFactor : FOG;
};

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
	VS_STANDARD_OUTPUT output;

	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.tangentW = mul(input.tangent, (float3x3)gmtxWorld);
	output.bitangentW = mul(input.bitangent, (float3x3)gmtxWorld);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;

    //포그 계수... 1: 투명하다 ~ 0: 탁하다
    float fogEnd = 30000;
    float fogStart = 100;
    output.fogFactor = saturate((fogEnd - gvCameraPosition.z) / (fogEnd- fogStart));
       
	return(output);
}

float4 PSStandard(VS_STANDARD_OUTPUT input) : SV_TARGET
{ 
	// 임시로 사용할 컬러 색깔
    float4 TESTColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
   
    if (TESTColor.a < 0.1 && TESTColor.r < 0.1 && TESTColor.g < 0.1 && TESTColor.b < 0.1)
        TESTColor = float4(1.f, 1.f, 1.f, 1.f);
    else if (TESTColor.a < 0.5) discard;
    
	//float4 cAlbedoColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_ALBEDO_MAP) cAlbedoColor = gtxtAlbedoTexture.Sample(gssWrap, input.uv);
	//float4 cSpecularColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_SPECULAR_MAP) cSpecularColor = gtxtSpecularTexture.Sample(gssWrap, input.uv);
	//float4 cNormalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_NORMAL_MAP) cNormalColor = gtxtNormalTexture.Sample(gssWrap, input.uv);
	//float4 cMetallicColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_METALLIC_MAP) cMetallicColor = gtxtMetallicTexture.Sample(gssWrap, input.uv);
	//float4 cEmissionColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (gnTexturesMask & MATERIAL_EMISSION_MAP) cEmissionColor = gtxtEmissionTexture.Sample(gssWrap, input.uv);

	float3 normalW;
	//float4 cColor = cAlbedoColor + cSpecularColor + cMetallicColor + cEmissionColor;
	//if (gnTexturesMask & MATERIAL_NORMAL_MAP)
	//{
	//	float3x3 TBN = float3x3(normalize(input.tangentW), normalize(input.bitangentW), normalize(input.normalW));
	//	float3 vNormal = normalize(cNormalColor.rgb * 2.0f - 1.0f); //[0, 1] → [-1, 1]
	//	normalW = normalize(mul(vNormal, TBN));
	//}
	//else
	//{
		normalW = normalize(input.normalW);
	//}

	float4 cIllumination = Lighting(input.positionW, normalW);
    float4 ContrastColor = lerp(TESTColor, cIllumination, 0.5f); // 명암처리된 픽셀 색깔

    float4 fogColor = float4(0.5, 0.5, 0.5, 1.0f);
    
    float4 finalColor = input.fogFactor * ContrastColor + (1.0 - input.fogFactor) * fogColor;

    return finalColor;
}
 

// Instancing ////////////////////////////////////////////////////////////
struct VS_INSTANCING_OUTPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

VS_STANDARD_OUTPUT VSStandardInstancing(VS_INSTANCING_OUTPUT input, uint nInstanceID : SV_InstanceID)
{
	VS_STANDARD_OUTPUT output;

    output.positionW = mul(float4(input.position, 1.0f), gmtxInstancingWorld[nInstanceID].m_mtxWorld).xyz;
    output.normalW = mul(input.normal, (float3x3) gmtxInstancingWorld[nInstanceID].m_mtxWorld);
    output.tangentW = mul(input.tangent, (float3x3) gmtxInstancingWorld[nInstanceID].m_mtxWorld);
    output.bitangentW = mul(input.bitangent, (float3x3) gmtxInstancingWorld[nInstanceID].m_mtxWorld);
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.uv = input.uv;
    
    
    // Zfar = 포그 시작 범위
    // Znear = 포그 끝 범위

    // Z = 뷰 좌표계에서의 정점의 거리
      
    // f = (Zfar - Z) / (Zfar - Znear) = Zfar / (Zfar - Znear) + Z * (-1 / (Zfar - Znear))
    //포그 계수... 1: 투명하다 ~ 0: 탁하다
    //float Z = length();
    //output.fogFactor = saturate((fogEnd - gvCameraPosition.z) / (fogEnd - fogStart));
    
    output.fogFactor = 0;

	return(output);
}
