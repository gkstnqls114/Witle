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
};


struct VS_SKINNED_STANDARD_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    int4 indices : BONEINDEX;
    float4 weights : BONEWEIGHT;
};


//  
// 현재 여기서 view는 조명 위치에서의 view입니다...
//

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
    VS_STANDARD_OUTPUT output;

    // 현재 여기에서 view 는 조명 위치에서의 matrix... 
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
     
    return (output);
}

VS_STANDARD_OUTPUT VSSkinned(VS_SKINNED_STANDARD_INPUT input)
{ 
    VS_STANDARD_OUTPUT output;
     
    float4x4 mtxVertexToBoneWorld = (float4x4) 0.0f;
    for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
    { 
        mtxVertexToBoneWorld += input.weights[i] * mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
    }
    float4 posW = mul(float4(input.position, 1.0f), mtxVertexToBoneWorld);
    output.positionW = posW.xyz;
    output.position = mul(mul(posW, gmtxView), gmtxProjection); 

    return (output);
}
 