#pragma once

// 
// MyConstants.h
// 전처리문을 정의하는 헤더 파일
// 

//// Enum Type ////////////////////////////////////
enum MESH_TYPE_ID
{
	CUBE_MESH,
	TERRAIN_MESH,
	QUADTREE_TERRAIN_MESH
};

enum LIGHT_TYPE
{
	POINT_LIGHT,
	SPOT_LIGHT,
	DIRECTIONAL_LIGHT
};
//// Enum Type ////////////////////////////////////


#define EPSILON				1.0e-10f

#define MOUSE_WHEEL_UP		120
#define MOUSE_WHEEL_DOWN	-120
#define MOUSE_WHEEL_STOP	0

#define CAMERA_FAR			50000.F 

/////////////////////////////////////////////// define 정의
#define _WITH_DEBUG_TERRAIN_MAX_POS
// #define CHECK_SUBVIEWS
#define CHECK_FRAMERATE // 정의한경우, 프레임 레이트가 상단 타이틀에 보인다.

// #define _WITH_DEBUG_FRAME_HIERARCHY
/////////////////////////////////////////////// define 정의


//// Root Parameter Index /////////////////////////////////////////// 
#define ROOTPATAMETER_COUNT 17

const UINT ROOTPARAMETER_WORLD = 0;
const UINT ROOTPARAMETER_CAMERA = 1;
const UINT ROOTPARAMETER_MATERIALS = 2;
const UINT ROOTPARAMETER_LIGHTS = 3;
const UINT ROOTPARAMETER_COLOR = 16;
const UINT ROOTPARAMETER_TEXTURE = 4;
const UINT ROOTPARAMETER_TEXTUREBASE = 5;
const UINT ROOTPARAMETER_TEXTUREDETAIL = 6;
const UINT ROOTPARAMETER_ALBEDO = 7;
const UINT ROOTPARAMETER_SPECULAR = 8;
const UINT ROOTPARAMETER_NORMAL = 9;
const UINT ROOTPARAMETER_METALLIC = 10;
const UINT ROOTPARAMETER_EMISSION_1 = 14;
const UINT ROOTPARAMETER_EMISSION_2 = 15;
const UINT ROOTPARAMETER_EMISSION_3 = 13;
const UINT ROOTPARAMETER_SKINNEDBONEOFFSET = 11;
const UINT ROOTPARAMETER_SKINNEDBONETRANSFORM = 12;
//// Root Parameter Index /////////////////////////////////////////// 


//// Mesh File Load /////////////////////////////////////////// 
#define VERTEXT_POSITION				0x0001
#define VERTEXT_COLOR					0x0002
#define VERTEXT_NORMAL					0x0004
#define VERTEXT_TANGENT					0x0008
#define VERTEXT_TEXTURE_COORD0			0x0010
#define VERTEXT_TEXTURE_COORD1			0x0020

#define VERTEXT_BONE_INDEX_WEIGHT		0x1000

#define VERTEXT_TEXTURE					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_DETAIL					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TEXTURE			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_TANGENT_TEXTURE	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_DETAIL			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TANGENT__DETAIL	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)

#define SKINNED_ANIMATION_BONES		128

#define _WITH_DEBUG_CALLBACK_DATA
////  Mesh File Load ///////////////////////////////////////////


//// Load Material /////////////////////////////////////////// 
#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40
//// Load Material /////////////////////////////////////////// 


//// Animation Type /////////////////////////////////////////// 
#define ANIMATION_TYPE_ONCE			0
#define ANIMATION_TYPE_LOOP			1
#define ANIMATION_TYPE_PINGPONG		2

#define ANIMATION_CALLBACK_EPSILON	0.015f
//// Animation Type /////////////////////////////////////////// 


//// Animation Type /////////////////////////////////////////// 
//#define _WITH_ANIMATION_SRT
#define _WITH_ANIMATION_INTERPOLATION
// 위 둘 중에 하나만 define
//// Animation Type /////////////////////////////////////////// 


//// 조명 및 재질 ///////////////////////////////////////////  
#define MAX_LIGHTS 8
#define MAX_MATERIALS 8
//// 조명 및 재질 ///////////////////////////////////////////  


//// HeightMapImage.h 관련 define ////////////////////////
#define _WITH_APPROXIMATE_OPPOSITE_CORNER
//// HeightMapImage.h 관련 define ////////////////////////


//// Object.h 교수님 코드 관련 define ////////////////////////
#define _WITH_DISPLAY_TEXTURE_NAME
//// Object.h 교수님 코드 관련 define ////////////////////////


//// 조작키 /////////////////////////////////////////// 
#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20
//// 조작키 /////////////////////////////////////////// 


//// Load Texture /////////////////////////////////////////// 
#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05
//// Load Texture /////////////////////////////////////////// 


//정점의 색상을 무작위로(Random) 설정하기 위해 사용한다. 각 정점의 색상은 난수(Random Number)를 생성하여 지정한다.
#define RANDOM_COLOR XMFLOAT4{rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)}
