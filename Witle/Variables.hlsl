
// ��Ʈ ���
cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
}

// ��� ����
cbuffer cbGameObjectInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gvCameraPosition : packoffset(c8);
}

cbuffer cbLightInfoForShadow : register(b9)
{
    matrix gmtxLightView : packoffset(c0);
    matrix gmtxLightProjection : packoffset(c4); 
    matrix gShadowTransform : packoffset(c8); 
} 

cbuffer cbPlayerLightInfoForShadow : register(b10)
{ 
    matrix gmtxPlayerLightView : packoffset(c0);
    matrix gmtxPlayerLightProjection : packoffset(c4);
    matrix gPlayerShadowTransform : packoffset(c8);
}

// ��Ʈ ���
cbuffer cbPickingPoint : register(b4)
{
    float2 pickingPoint : packoffset(c0);
}

// ��Ʈ ���
cbuffer cbHPpercentage : register(b5)
{
    float HPpercentage : packoffset(c0);
}

// ��Ʈ ���
cbuffer cbTotalTime : register(b6)
{
    float totalTime : packoffset(c0);
}


SamplerState gWrapSamplerState : register(s0);
SamplerState gssClamp          : register(s1);
SamplerComparisonState gssPCFSampler     : register(s2);

Texture2D gtxtTexture : register(t0);
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);
Texture2D<float> gtxtShadow : register(t3);
Texture2D<float> gtxtPlayerShadow : register(t4);

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

#define MAX_VERTEX_INFLUENCES			4
#define SKINNED_ANIMATION_BONES			128

cbuffer cbBoneOffsets : register(b7)
{
    float4x4 gpmtxBoneOffsets[SKINNED_ANIMATION_BONES];
};

cbuffer cbBoneTransforms : register(b8)
{
    float4x4 gpmtxBoneTransforms[SKINNED_ANIMATION_BONES];
};

Texture2D gtxtAlbedoTexture : register(t6);
Texture2D gtxtSpecularTexture : register(t7);
Texture2D gtxtNormalTexture : register(t8);
Texture2D gtxtMetallicTexture : register(t9);
Texture2D gtxtEmissionTexture : register(t10);
Texture2D gtxtDetailAlbedoTexture : register(t11);
Texture2D gtxtDetailNormalTexture : register(t12);
TextureCube gtxtSkyCubeTexture : register(t13);
 
// ���
RWTexture2D<float4> gtxtRWOutput : register(u0);
RWStructuredBuffer<float> gAverageLum : register(u1);
RWStructuredBuffer<float> gAverageValues1D : register(u2);

//Res: �� ������ ���̿� �ʺ� 4�� ���� ��
static const uint2 Res = uint2(1280 / 4, 720 / 4);

// �� ������ ���̿� �ʺ� ���� �� 16���� ���� ��
static const uint Domain = uint(1280 * 720 / 16);

// �� ������ ���̿� �ʺ� ���� �� 16���� ���� �� 1024�� ���� ��
static const uint GroupSize = uint(1280 * 720 / 16 * 1024);

// �ֵ� ����� ���� ���
static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

//�ν��Ͻ� �����͸� ���� ����ü
struct INSTANCING_TRANSFORM
{
    matrix m_mtxWorld;
};

StructuredBuffer<INSTANCING_TRANSFORM> gmtxInstancingWorld : register(t14);

float NormalizeSpecPower(float specpower)
{
    float2 specpowerRange = float2(1, 2);
    return (specpower - specpowerRange.x) / specpowerRange.y;
}

struct PS_OUTPUT_FOR_GBUFFERS
{ 
    float4 ColorSpecInt : SV_TARGET0;
    float4 Normal : SV_TARGET1; // w������Ʈ�� �������
    float4 SpecPow : SV_TARGET2;
};

float CalcShadowFactor(float4 shadowPosH)
{
    // Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;

    // Depth in NDC space.
    float depth = shadowPosH.z;

    uint width, height, numMips;
    gtxtShadow.GetDimensions(0, width, height, numMips);

    // Texel size.
    float dx = 1.0f / (float) width;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += gtxtShadow.SampleCmpLevelZero(gssPCFSampler,
            shadowPosH.xy + offsets[i], depth).r;
    }
    
    return percentLit / 9.0f;
}


float CalcPlayerShadowFactor(float4 shadowPosH)
{
    // Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;

    // Depth in NDC space.
    float depth = shadowPosH.z;

    uint width, height, numMips;
    gtxtPlayerShadow.GetDimensions(0, width, height, numMips);

    // Texel size.
    float dx = 1.0f / (float) width;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += gtxtPlayerShadow.SampleCmpLevelZero(gssPCFSampler,
            shadowPosH.xy + offsets[i], depth).r;
    }
    
    return percentLit / 9.0f;
}

