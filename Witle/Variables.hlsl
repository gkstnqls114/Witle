
// 루트 상수
cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
}

// 상수 버퍼
cbuffer cbGameObjectInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gvCameraPosition : packoffset(c8);
}

// 루트 상수
cbuffer cbPickingPoint : register(b4)
{
    float2 pickingPoint : packoffset(c0);
}

// 루트 상수
cbuffer cbHPpercentage : register(b5)
{
    float HPpercentage : packoffset(c0);
}

// 루트 상수
cbuffer cbElapsedTime : register(b6)
{
    float ElapsedTime : packoffset(c0);
}

SamplerState gWrapSamplerState : register(s0);
SamplerState gssClamp          : register(s1);

Texture2D gtxtTexture : register(t0);
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);

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
 
//인스턴싱 데이터를 위한 구조체
struct INSTANCING_TRANSFORM
{
    matrix m_mtxWorld;
};

StructuredBuffer<INSTANCING_TRANSFORM> gmtxInstancingWorld : register(t14);
