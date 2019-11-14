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


struct VS_TERRAIN_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

struct VS_STANDARD_OUTPUT
{
    float4 position : SV_POSITION;
    float4 positionW : POSITION;
    float2 uv : TEXCOORD;
};



//  
// 현재 여기서 view는 조명 위치에서의 view입니다...
//

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
    VS_STANDARD_OUTPUT output;

    // 현재 여기에서 view 는 조명 위치에서의 matrix... 

    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld); 
    output.position = mul(mul(output.positionW, gmtxLightView), gmtxLightProjection);
    output.uv = input.uv;
     
	// Output vertex attributes for interpolation across triangle.
    //float4 texC = mul(float4(input.uv, 0.0f, 1.0f), gTexTransform);
    //vout.TexC = mul(texC, matData.MatTransform).xy;
	
    return (output);
}


VS_STANDARD_OUTPUT VSStandardForPlayer(VS_STANDARD_INPUT input)
{
    VS_STANDARD_OUTPUT output;

    // 현재 여기에서 view 는 조명 위치에서의 matrix... 

    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld);

    output.position = mul(mul(output.positionW, gmtxPlayerLightView), gmtxPlayerLightProjection); 
    output.TexC = input.uv;
	
    return (output);
}


VS_STANDARD_OUTPUT VSSkinnedForPlayer(VS_SKINNED_STANDARD_INPUT input)
{
    VS_STANDARD_OUTPUT output;
     
    float4x4 mtxVertexToBoneWorld = (float4x4) 0.0f;
    for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
    {
        mtxVertexToBoneWorld += input.weights[i] * mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
    }
    float4 posW = mul(float4(input.position, 1.0f), mtxVertexToBoneWorld);
    output.positionW = posW;
    output.position = mul(mul(posW, gmtxPlayerLightView), gmtxPlayerLightProjection);
    output.TexC = input.uv;

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
    output.positionW = posW;
    output.position = mul(mul(posW, gmtxLightView), gmtxLightProjection);
    output.TexC = input.uv;

    return (output);
}
 

VS_STANDARD_OUTPUT VSStandardInstancing(VS_STANDARD_INPUT input, uint nInstanceID : SV_InstanceID)
{
    VS_STANDARD_OUTPUT output;

    output.positionW = mul(float4(input.position, 1.0f), gmtxInstancingWorld[nInstanceID].m_mtxWorld); 
    output.position = mul(mul(output.positionW, gmtxLightView), gmtxLightProjection);
    output.TexC = input.uv;

    return (output);
}


VS_STANDARD_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
    VS_STANDARD_OUTPUT output;

// #ifdef _WITH_CONSTANT_BUFFER_SYNTAX
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
//#else
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld);
    output.position = mul(mul(output.positionW, gmtxLightView), gmtxLightProjection);
//#endif
//#endif 
    output.TexC = input.uv0;

    return (output);
}