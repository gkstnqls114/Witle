 
#include "Variables.hlsl"
#include "Light.hlsl"
 
struct VS_SKINNED_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	int4  indices : BONEINDEX;
	float4 weights : BONEWEIGHT;
};
 
struct VS_STANDARD_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
};

VS_STANDARD_OUTPUT VSSkinnedAnimationStandard(VS_SKINNED_STANDARD_INPUT input)
{ 
	//VS_STANDARD_OUTPUT output;
	 
	//float3 positionW = float3(0.0f, 0.0f, 0.0f);
    
 //   float4x4 mtxVertexToBoneWorld = (float4x4) 0.0f;
	//for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
	//{ 
 //       mtxVertexToBoneWorld = mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
 //       positionW += input.weights[i] * mul(float4(input.position, 1.0f), mtxVertexToBoneWorld).xyz;
 //   } 
      
 //   output.positionW = positionW;
	//output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
	//output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	//output.tangentW = mul(input.tangent, (float3x3)gmtxWorld);
	//output.bitangentW = mul(input.bitangent, (float3x3)gmtxWorld);
	//output.uv = input.uv;
	 
	//return(output);



    VS_STANDARD_OUTPUT output;

	//output.positionW = float3(0.0f, 0.0f, 0.0f);
	//output.normalW = float3(0.0f, 0.0f, 0.0f);
	//output.tangentW = float3(0.0f, 0.0f, 0.0f);
	//output.bitangentW = float3(0.0f, 0.0f, 0.0f);
	//matrix mtxVertexToBoneWorld;
	//for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
	//{
	//	mtxVertexToBoneWorld = mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
	//	output.positionW += input.weights[i] * mul(float4(input.position, 1.0f), mtxVertexToBoneWorld).xyz;
	//	output.normalW += input.weights[i] * mul(input.normal, (float3x3)mtxVertexToBoneWorld);
	//	output.tangentW += input.weights[i] * mul(input.tangent, (float3x3)mtxVertexToBoneWorld);
	//	output.bitangentW += input.weights[i] * mul(input.bitangent, (float3x3)mtxVertexToBoneWorld);
	//}
	 
    float4x4 mtxVertexToBoneWorld = (float4x4) 0.0f;
    for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
    {
		//		mtxVertexToBoneWorld += input.weights[i] * gpmtxBoneTransforms[input.indices[i]];
        mtxVertexToBoneWorld += input.weights[i] * mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
    }
    float4 posW = mul(float4(input.position, 1.0f), mtxVertexToBoneWorld);
    output.positionW = posW.xyz;
    output.normalW = mul(input.normal, (float3x3) mtxVertexToBoneWorld).xyz;
    output.tangentW = mul(input.tangent, (float3x3) mtxVertexToBoneWorld).xyz;
    output.bitangentW = mul(input.bitangent, (float3x3) mtxVertexToBoneWorld).xyz;

	//	output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject).xyz; 
    output.position = mul(mul(posW, gmtxView), gmtxProjection);
    output.uv = input.uv;

	return(output);
}

// 명암 처리 하지않는다.
float4 PSStandardNot(VS_STANDARD_OUTPUT input) : SV_TARGET
{
    float4 TESTColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    
    TESTColor *= 1.5;

    return TESTColor;
}

float4 PSStandard(VS_STANDARD_OUTPUT input) : SV_TARGET
{
// 임시로 사용할 컬러 색깔
    
    float4 TESTColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);
    if (TESTColor.a < 0.8)
        discard;
    
    float3 normalW = float3(0.f, 0.f, 0.f); 
    normalW = normalize(input.normalW);
    float4 cIllumination = Lighting(input.positionW, normalW);
     
    return (lerp(TESTColor, cIllumination, 0.5f));
} 


PS_OUTPUT_FOR_GBUFFERS PSStandardForGBuffers(VS_STANDARD_OUTPUT input)
{
    PS_OUTPUT_FOR_GBUFFERS output;
      
	// 임시로 사용할 컬러 색깔
    float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv); 

    float depth = input.position.z / input.position.w;
    // gbuffer 구조체에 패킹
    float SpecPowerNorm = NormalizeSpecPower(1); // 스페큘러 파워 정규화
    float SpecIntensity = 1; 
    output.ColorSpecInt = float4(cColor.rgb, SpecIntensity);
    output.Normal = float4(input.normalW.xyz * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(depth, depth, depth, 0);

    return (output);
}