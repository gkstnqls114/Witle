
// 루트 상수
cbuffer cbGameObjectInfo : register(b0)
{
	matrix gmtxWorld : packoffset(c0);
}

// 상수 버퍼
cbuffer cbCameraInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
	float3 gvCameraPosition : packoffset(c8);
}

// 루트 상수
cbuffer cbGameObjectInfo : register(b4)
{
	float3 testcolor: packoffset(c0);
}

//#include "Light.hlsl"
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
#define MAX_LIGHTS			8 
#define MAX_MATERIALS		8 

#define POINT_LIGHT			0
#define SPOT_LIGHT			1
#define DIRECTIONAL_LIGHT	2

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES
//#define _WITH_REFLECT

struct MATERIAL
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular; //a = power
	float4				m_cEmissive;
};

struct LIGHT
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular;
	float3				m_vPosition;
	float 				m_fFalloff;
	float3				m_vDirection;
	float 				m_fTheta; //cos(m_fTheta)
	float3				m_vAttenuation;
	float				m_fPhi; //cos(m_fPhi)
	bool				m_bEnable;
	int 				m_nType;
	float				m_fRange;
	float				padding;
};

cbuffer cbMaterial : register(b2)
{
	MATERIAL			gMaterials;
};

cbuffer cbLights : register(b3)
{
	LIGHT				gLights[MAX_LIGHTS];
	float4				gcGlobalAmbientLight;
};

float4 DirectionalLight(int nIndex, float3 vNormal, float3 vToCamera)
{
	float3 vToLight = -gLights[nIndex].m_vDirection;
	float fDiffuseFactor = dot(vToLight, vNormal);
	float fSpecularFactor = 0.0f;
	if (fDiffuseFactor > 0.0f)
	{
		if (gMaterials.m_cSpecular.a != 0.0f)
		{
#ifdef _WITH_REFLECT
			float3 vReflect = reflect(-vToLight, vNormal);
			fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterials.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
			float3 vHalf = normalize(vToCamera + vToLight);
#else
			float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
			fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterials.m_cSpecular.a);
#endif
		}
	}

	return((gLights[nIndex].m_cAmbient * gMaterials.m_cAmbient) + (gLights[nIndex].m_cDiffuse * fDiffuseFactor * gMaterials.m_cDiffuse) + (gLights[nIndex].m_cSpecular * fSpecularFactor * gMaterials.m_cSpecular));
}

float4 PointLight(int nIndex, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	float3 vToLight = gLights[nIndex].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gLights[nIndex].m_fRange)
	{
		float fSpecularFactor = 0.0f;
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (gMaterials.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
				fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterials.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
				fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterials.m_cSpecular.a);
#endif
			}
		}
		float fAttenuationFactor = 1.0f / dot(gLights[nIndex].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));

		return(((gLights[nIndex].m_cAmbient * gMaterials.m_cAmbient) + (gLights[nIndex].m_cDiffuse * fDiffuseFactor * gMaterials.m_cDiffuse) + (gLights[nIndex].m_cSpecular * fSpecularFactor * gMaterials.m_cSpecular)) * fAttenuationFactor);
	}
	return(float4(0.0f, 0.0f, 0.0f, 0.0f));
}

float4 SpotLight(int nIndex, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	float3 vToLight = gLights[nIndex].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gLights[nIndex].m_fRange)
	{
		float fSpecularFactor = 0.0f;
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (gMaterials.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
				fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterials.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
				fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterials.m_cSpecular.a);
#endif
			}
		}
#ifdef _WITH_THETA_PHI_CONES
		float fAlpha = max(dot(-vToLight, gLights[nIndex].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gLights[nIndex].m_fPhi) / (gLights[nIndex].m_fTheta - gLights[nIndex].m_fPhi)), 0.0f), gLights[nIndex].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), gLights[i].m_fFalloff);
#endif
		float fAttenuationFactor = 1.0f / dot(gLights[nIndex].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));

		return(((gLights[nIndex].m_cAmbient * gMaterials.m_cAmbient) + (gLights[nIndex].m_cDiffuse * fDiffuseFactor * gMaterials.m_cDiffuse) + (gLights[nIndex].m_cSpecular * fSpecularFactor * gMaterials.m_cSpecular)) * fAttenuationFactor * fSpotFactor);
	}
	return(float4(0.0f, 0.0f, 0.0f, 0.0f));
}

float4 Lighting(float3 vPosition, float3 vNormal)
{
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
	float3 vToCamera = normalize(vCameraPosition - vPosition);

	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		if (gLights[i].m_bEnable)
		{
			if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
			{
				cColor += DirectionalLight(i, vNormal, vToCamera);
			}
			else if (gLights[i].m_nType == POINT_LIGHT)
			{
				cColor += PointLight(i, vPosition, vNormal, vToCamera);
			}
			else if (gLights[i].m_nType == SPOT_LIGHT)
			{
				cColor += SpotLight(i, vPosition, vNormal, vToCamera);
			}
		}
	}
	cColor += (gcGlobalAmbientLight * gMaterials.m_cAmbient);
	cColor.a = gMaterials.m_cDiffuse.a;

	return(cColor);
}


// #define _WITH_VERTEX_LIGHTING

struct VertexIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	//	nointerpolation float3 normalW : NORMAL;
	float4 cubeColor : COLOR0;
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR1;
#endif
};

VertexOut VS(VertexIn input)
{
	VertexOut output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxWorld);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	// output.uv = input.uv;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif
	output.cubeColor = input.color;
	return(output);

	//VertexOut vout;

	//vout.Pos = mul(mul(mul(float4(input.Pos, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	//vout.Color = input.Color; 
	//vout.normal = input.normal;

	//return vout;
}

float4 PS(VertexOut input) : SV_TARGET
{
	// float4 cColor = gtxtDiffuse.Sample(gSamplerState, input.uv);
	float4 cColor;
	if (testcolor.x == 0.f)
	{
		cColor = input.cubeColor;
	}
	else
	{
		cColor = float4(testcolor, 1.0f);
	}
#ifdef _WITH_VERTEX_LIGHTING
	float4 cIllumination = input.color;
#else
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
#endif
	return(cColor * cIllumination);


}


//VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
//{
//	VS_TEXTURED_LIGHTING_OUTPUT output;
//
//	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
//	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
//	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
//	output.uv = input.uv;
//#ifdef _WITH_VERTEX_LIGHTING
//	output.normalW = normalize(output.normalW);
//	output.color = Lighting(output.positionW, output.normalW);
//#endif
//	return(output);
//}
//
//float4 PSTexturedLighting(VS_TEXTURED_LIGHTING_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
//{
//	float4 cColor = gtxtDiffuse.Sample(gSamplerState, input.uv);
//#ifdef _WITH_VERTEX_LIGHTING
//	float4 cIllumination = input.color;
//#else
//	input.normalW = normalize(input.normalW);
//	float4 cIllumination = Lighting(input.positionW, input.normalW);
//#endif
//	return(cColor * cIllumination);
//}