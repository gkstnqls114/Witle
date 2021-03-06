// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

// Windows 헤더 파일:
#include <windows.h>

// 콘솔창에서 확인을 위해 헤더 추가
#include <iostream>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <wrl.h>
#include <shellapi.h>
#include <functional>
#include <crtdbg.h>
#include <random>
#include <chrono>
#include <queue>

// DirectX 12
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace  Microsoft::WRL;

// 매크로 상수를 위한 헤더
#include "MyConstants.h"
#include "MyStructs.h"
#include "MyNames.h"
#include "MyAnimationInfos.h"
  

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib")

#pragma warning(disable:4996)

// 디버그 모드인 경우, 콘솔 창을 띄워 확인합니다.
// #if defined(_DEBUG)
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
// #endif


inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception();
	}
}

inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

inline bool IsZero(float fValue) { return((fabsf(fValue) < EPSILON)); }
inline bool IsZero(float fValue, float fEpsilon) { return((fabsf(fValue) < fEpsilon)); }

inline bool IsEqual(float fA, float fB) { return(::IsZero(fA - fB)); }
inline bool IsEqual(float fA, float fB, float fEpsilon) { return(::IsZero(fA - fB, fEpsilon)); }

inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
inline void Swap(float *pfS, float *pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }

namespace Quaternion
{
	// roll : x축 ,  pitch : y축 , yaw : z축
	inline XMFLOAT4  ToQuaternion(float roll, float  pitch, float  yaw)  // roll (X), pitch (Y), yaw (Z) 
	{
		XMFLOAT4 q; 
		XMStoreFloat4(&q, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(roll), XMConvertToRadians(pitch), XMConvertToRadians(yaw)));
		return q;
	}
}
  
namespace Vector2
{
	inline XMFLOAT2 Add(const XMFLOAT2& xmf3Vector1, const XMFLOAT2& xmf3Vector2)
	{
		XMFLOAT2 xmf3Result;
		XMStoreFloat2(&xmf3Result, XMLoadFloat2(&xmf3Vector1) +
			XMLoadFloat2(&xmf3Vector2));
		return(xmf3Result);
	}
}

namespace Vector3
{

	inline bool IsZero(XMFLOAT3& xmf3Vector)
	{
		if (::IsZero(xmf3Vector.x) && ::IsZero(xmf3Vector.y) && ::IsZero(xmf3Vector.z)) return(true);
		return(false);
	}

#if _DEBUG
	inline void Show(const XMFLOAT3& xmf3Vector) noexcept
	{
		std::cout << xmf3Vector.x << " " << xmf3Vector.y << " " << xmf3Vector.z << std::endl;
	}
#endif
	inline XMVECTOR XMFloat3ToVector(const XMFLOAT3& xmf3Vector)
	{
		XMVECTOR xmVector;
		xmVector = XMLoadFloat3(&xmf3Vector);
		return xmVector;
	}

	inline XMFLOAT3 XMVectorToFloat3(const XMVECTOR& xmvVector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, xmvVector);
		return(xmf3Result);
	}


	inline XMFLOAT3 ScalarProduct(const XMFLOAT3& xmf3Vector, float fScalar, bool bNormalize =
		true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)) *
				fScalar);
		else
			XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector) * fScalar);
		return(xmf3Result);
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) +
			XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, float fScalar)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + (XMLoadFloat3(&xmf3Vector2)
			* fScalar));
		return(xmf3Result);
	}

	inline XMFLOAT3 Subtract(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) -
			XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	inline float DotProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1),
			XMLoadFloat3(&xmf3Vector2)));
		return(xmf3Result.x);
	}

	inline XMFLOAT3 CrossProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, bool
		bNormalize = true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result,
				XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&xmf3Vector1),
					XMLoadFloat3(&xmf3Vector2))));
		else
			XMStoreFloat3(&xmf3Result, XMVector3Cross(XMLoadFloat3(&xmf3Vector1),
				XMLoadFloat3(&xmf3Vector2)));
		return(xmf3Result);
	}

	inline XMFLOAT3 Normalize(const XMFLOAT3& xmf3Vector) // 와 const로 바꾸니까 해결됐잖아! 지금까지 내가 뭘 한거지?
	{
		XMFLOAT3 m_xmf3Normal;
		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
		return(m_xmf3Normal);
	}
	 
	inline XMFLOAT3 Random(bool isZeroX = false, bool isZeroY = false, bool isZeroZ = false)
	{
		static std::random_device rd;
		static  std::mt19937 mersenne(rd()); 
		static  std::uniform_real_distribution<> die(-1, 1);

		float x = 0, y = 0, z = 0;
		if (!isZeroX) x = die(mersenne);
		if (!isZeroY) y = die(mersenne);
		if (!isZeroZ) z = die(mersenne);

		return Vector3::Normalize(XMFLOAT3(x, y, z));
	}

	inline float Length(const XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
		return(xmf3Result.x);
	}

	// a와 b사이의 거리
	inline float Length(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&Vector3::Subtract(a, b))));
		return(xmf3Result.x);
	}

	inline float Angle(const XMVECTOR& xmvVector1, const XMVECTOR& xmvVector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2);
		return(XMConvertToDegrees(acosf(XMVectorGetX(xmvAngle))));
	}

	inline float Angle(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMVECTOR a = XMLoadFloat3(&xmf3Vector1);
		XMVECTOR b = XMLoadFloat3(&xmf3Vector2);
		return Angle(a, b);
	}

	inline float AngleToRadian(const XMVECTOR& xmvVector1, const XMVECTOR& xmvVector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2);
		return XMVectorGetX(xmvAngle);
	}

	inline float AngleToRadian(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMVECTOR a = XMLoadFloat3(&xmf3Vector1);
		XMVECTOR b = XMLoadFloat3(&xmf3Vector2);
		return AngleToRadian(a, b);
	}

	inline XMFLOAT3 TransformNormal(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector),
			xmmtxTransform));
		return(xmf3Result);
	}

	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, const XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
		return(xmf3Result);
	}

	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMMATRIX a = XMLoadFloat4x4(&xmmtx4x4Matrix);
		return(TransformCoord(xmf3Vector, a));
	}

	inline float AngleBetweenVector(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		float angle = 0;
		angle = acos(DotProduct(xmf3Vector1, xmf3Vector2) / (Length(xmf3Vector1) + Length(xmf3Vector2)));
		return angle;
	}

	inline XMFLOAT3 Reflect(const XMFLOAT3& normal, const XMFLOAT3& vector)
	{
		return ScalarProduct(normal, DotProduct(ScalarProduct(vector, -1, false), normal));
	}

	inline XMFLOAT3 Sliding(const XMFLOAT3& normal, const XMFLOAT3& vector)
	{
		return Add(vector, Reflect(normal, vector));
	}
	inline XMFLOAT3 Sliding(const XMFLOAT4& normal, const XMFLOAT3& vector)
	{
		return Add(vector, Reflect(XMFLOAT3{ normal.x, normal.y, normal.z }, vector));
	}
}


//4차원 벡터의 연산
namespace Vector4
{

#if _DEBUG
	inline void Show(const XMFLOAT4& xmf4Vector) noexcept
	{
		std::cout << xmf4Vector.x << " " << xmf4Vector.y << " " << xmf4Vector.z << " " << xmf4Vector.w << std::endl;
	}
#endif

	inline XMVECTOR XMFloat4ToVector(const XMFLOAT4& xmf3Vector)
	{
		XMVECTOR xmVector;
		xmVector = XMLoadFloat4(&xmf3Vector);
		return xmVector;
	}


	inline XMFLOAT4 XMVectorToFloat4(const XMVECTOR& xmvVector)
	{
		XMFLOAT4 xmf3Result;
		XMStoreFloat4(&xmf3Result, xmvVector);
		return(xmf3Result);
	}

	//4-차원 벡터와 스칼라(실수)의 곱을 반환하는 함수이다.
	inline XMFLOAT4 Multiply(float fScalar, XMFLOAT4& xmf4Vector)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, fScalar * XMLoadFloat4(&xmf4Vector));
		return(xmf4Result);
	}

	inline XMFLOAT4 Add(const XMFLOAT4& xmf4Vector1, const XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) +
			XMLoadFloat4(&xmf4Vector2));
		return(xmf4Result);
	}
}

//행렬의 연산
namespace Matrix4x4
{
#if _DEBUG

	inline void Show(const XMFLOAT4X4& xmmtx4x4Matrix) noexcept
	{
		printf("%f\t %f\t %f\t %f\t\n", xmmtx4x4Matrix._11, xmmtx4x4Matrix._12, xmmtx4x4Matrix._13, xmmtx4x4Matrix._14);
		printf("%f\t %f\t %f\t %f\t\n", xmmtx4x4Matrix._21, xmmtx4x4Matrix._22, xmmtx4x4Matrix._23, xmmtx4x4Matrix._24);
		printf("%f\t %f\t %f\t %f\t\n", xmmtx4x4Matrix._31, xmmtx4x4Matrix._32, xmmtx4x4Matrix._33, xmmtx4x4Matrix._34);
		printf("%f\t %f\t %f\t %f\t\n", xmmtx4x4Matrix._41, xmmtx4x4Matrix._42, xmmtx4x4Matrix._43, xmmtx4x4Matrix._44);
		//std::cout << xmmtx4x4Matrix._11 << " " << xmmtx4x4Matrix._12 << " " << xmmtx4x4Matrix._13 << " " << xmmtx4x4Matrix._14 << std::endl;
		//std::cout << xmmtx4x4Matrix._21 << " " << xmmtx4x4Matrix._22 << " " << xmmtx4x4Matrix._23 << " " << xmmtx4x4Matrix._24 << std::endl;
		//std::cout << xmmtx4x4Matrix._31 << " " << xmmtx4x4Matrix._32 << " " << xmmtx4x4Matrix._33 << " " << xmmtx4x4Matrix._34 << std::endl;
		//std::cout << xmmtx4x4Matrix._41 << " " << xmmtx4x4Matrix._42 << " " << xmmtx4x4Matrix._43 << " " << xmmtx4x4Matrix._44 << std::endl;
	}
#endif

	inline XMFLOAT4X4 Zero()
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixSet(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Identity()
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixIdentity());
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 WorldMatrix(const XMFLOAT3& pos, const XMFLOAT3& right, const XMFLOAT3& up, const XMFLOAT3& look)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		xmmtx4x4Result._11 = right.x;  xmmtx4x4Result._12 = right.y;  xmmtx4x4Result._13 = right.z; xmmtx4x4Result._14 = 0;
		xmmtx4x4Result._21 = up.x;     xmmtx4x4Result._22 = up.y;     xmmtx4x4Result._23 = up.z;    xmmtx4x4Result._24 = 0;
		xmmtx4x4Result._31 = look.x;   xmmtx4x4Result._32 = look.y;   xmmtx4x4Result._33 = look.z;  xmmtx4x4Result._34 = 0;
		xmmtx4x4Result._41 = pos.x;    xmmtx4x4Result._42 = pos.y;    xmmtx4x4Result._43 = pos.z;   xmmtx4x4Result._44 = 1; 
		return(xmmtx4x4Result);
	}

	// 해당 회전량에 대한 회전 행렬을 새로 만든다. roll:z yaw:y pitch:x
	inline XMFLOAT4X4 RotateMatrix(float roll, float yaw, float pitch)
	{ 
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll)));
		return(xmmtx4x4Result);
	}

	// 해당 쿼터니언에 대한 회전 행렬을 새로 만든다.
	inline XMFLOAT4X4 RotateMatrix(const XMFLOAT4& Quaternion)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixRotationQuaternion(Vector4::XMFloat4ToVector(Quaternion)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Scale(XMFLOAT4X4& xmf4x4Matrix, float fScale)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmf4x4Matrix) * fScale);
		/*
				XMVECTOR S, R, T;
				XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&xmf4x4Matrix));
				S = XMVectorScale(S, fScale);
				T = XMVectorScale(T, fScale);
				R = XMVectorScale(R, fScale);
				//R = XMQuaternionMultiply(R, XMVectorSet(0, 0, 0, fScale));
				XMStoreFloat4x4(&xmf4x4Result, XMMatrixAffineTransformation(S, XMVectorZero(), R, T));
		*/
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmmtx4x4Matrix1, const XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) *
			XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmmtx4x4Matrix1, XMMATRIX& xmmtxMatrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * xmmtxMatrix2);
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMMATRIX& xmmtxMatrix1, const XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, xmmtxMatrix1 * XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmmtx4x4Result);
	}


	inline XMFLOAT4X4 Add(const XMFLOAT4X4& xmmtx4x4Matrix1, const XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) + XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Interpolate(XMFLOAT4X4& xmf4x4Matrix1, XMFLOAT4X4& xmf4x4Matrix2, float t)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMVECTOR S0, R0, T0, S1, R1, T1;
		XMMatrixDecompose(&S0, &R0, &T0, XMLoadFloat4x4(&xmf4x4Matrix1));
		XMMatrixDecompose(&S1, &R1, &T1, XMLoadFloat4x4(&xmf4x4Matrix2));
		XMVECTOR S = XMVectorLerp(S0, S1, t);
		XMVECTOR T = XMVectorLerp(T0, T1, t);
		XMVECTOR R = XMQuaternionSlerp(R0, R1, t);
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixAffineTransformation(S, XMVectorZero(), R, T));
		return(xmf4x4Result);
	}


	inline XMFLOAT4X4 Inverse(const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixInverse(NULL,
			XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Transpose(XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result,
			XMMatrixTranspose(XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 OrthographicLH(float viewWidth, float viewHeight, float NearZ, float FarZ)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixOrthographicLH(viewWidth, viewHeight,
			NearZ, FarZ));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ,
		float FarZ)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio,
			NearZ, FarZ));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 LookAtLH(const XMFLOAT3& xmf3EyePosition, const XMFLOAT3& xmf3LookAtPosition, const XMFLOAT3& xmf3UpDirection)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition),
			XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
		return(xmmtx4x4Result);
	}

}


namespace Plane
{
	// 어떤 점 point에 대하여 plane과의 거리
	inline float Distance(const XMFLOAT3& planeNormal, float d, const XMFLOAT3 point)
	{
		return fabsf(planeNormal.x * point.x + planeNormal.y * point.y + planeNormal.z * point.z + d) / Vector3::Length(point);
	}

	// ax +by + cx + d= 0 에서 d를 구한다.
	// planePos : plane위의 점
	inline float CaculateD(const XMFLOAT4& plane, const XMFLOAT3& planePos)
	{
		return -(plane.x * planePos.x) - (plane.y * planePos.y) - (plane.z * planePos.z);
	}

	// 평면과 점의 관계
	// value = 0 : 평면위
	// value > 0 : 평면 앞
	// value < 0 : 평면 뒤
	inline float IntersectRelation(const XMFLOAT3& planeNormal, float d, const XMFLOAT3& pos)
	{
		return Vector3::DotProduct(planeNormal, pos) + d;
	}

	inline bool IsBehind(const XMFLOAT3& planeNormal, float d, const XMFLOAT3& pos)
	{
		return (IntersectRelation(planeNormal, d, pos) < 0.f);
	}

	inline bool IsFront(const XMFLOAT3& planeNormal, float d, const XMFLOAT3& pos)
	{
		return (IntersectRelation(planeNormal, d, pos) > 0.f);
	}

	// 유한직선이 무한평면이 지나가는가?
	// plane 노말벡터(a,b,c) 지나는점 k일 때 d = -(a*kx + b*ky + c*kz) : ax + by + cx + d = 0 
	// d: 해당 plane의 원점까지의 거리, out_IntersectionPoint: 교점(외부로 반환)
	inline bool Intersect(const XMFLOAT3& planeNormal, float d, const XMFLOAT3& origin, const XMFLOAT3& direction, XMFLOAT3& out_IntersectionPoint)
	{
		float t = ((-Vector3::DotProduct(planeNormal, origin) - d)) / (Vector3::DotProduct(planeNormal, Vector3::Normalize(direction)));
		float Raydistance = Vector3::Length(direction);
		if (t >= 0.0f && t <= Raydistance)
		{
			out_IntersectionPoint = Vector3::Add(origin, Vector3::ScalarProduct(Vector3::Normalize(direction), t, false));
			return true;
		}
		else
		{
			return false;
		}

	}

	// plane을 생성하는 함수이다.
	inline XMFLOAT4 Plane(const XMFLOAT3& point, const XMFLOAT3& normal)
	{
		return Vector4::XMVectorToFloat4(XMPlaneFromPointNormal(Vector3::XMFloat3ToVector(point), Vector3::XMFloat3ToVector(Vector3::Normalize(normal))));
	}
}