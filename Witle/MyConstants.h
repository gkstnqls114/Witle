#pragma once

// 
// MyConstants.h
// 전처리문을 정의하는 헤더 파일
// 


//// Enum Type ////////////////////////////////////
enum ENUM_EFFECT
{
	EFFECT_NONE = -1,
	EFFECT_TEST
};

enum ENUM_SCENE
{
	SCENE_NONE = -1,
	SCENE_MAIN,
	SCENE_SKILLSELECT,
	SCENE_GAME,
	SCENE_WIN,
	SCENE_LOSE,
};

enum ENUM_SKILLTYPE
{
	SKILLTYPE_NONE = -1,
	SKILLTYPE_ATTACK,
	SKILLTYPE_RECOVERY,
	SKILLTYPE_BUFF,
	SKILLTYPE_DEBUFF
};

enum ENUM_SELECTABLESKILL
{
	SELECTABLESKILL_NONE = -1, // 반드시 -1
	SELECTABLESKILL_FIREBALL,
	SELECTABLESKILL_ICEBALL,
	SELECTABLESKILL_LIGHTNINGBALL, 
	SELECTABLESKILL_SHIELD,
	SELECTABLESKILL_BLESSING,
	SELECTABLESKILL_HEALING,
	//SELECTABLESKILL_METEOR,
	//SELECTABLESKILL_ICESPEAR

	// 최대 8개까지... 더이상 증가 X
	//SKILL_LIGHTNING,
	//SKILL_LIGHTARROW
}; 

enum ENUM_BALLEFFECT
{
	BALLEFFECT_FIRE,
	BALLEFFECT_ICE,
	BALLEFFECT_LIGHTNING
};

enum ENUM_MONSTER
{
	MONSTER_CREEPYMONSTER,
	MONSTER_MUSHROOM,
	MONSTER_SPACECATPINK,
	MONSTER_SPACECATGREEN,
	MONSTER_SPACECATBLUE,
	MONSTER_BOSSMONSTER
};

enum ENUM_SOUND
{
	SATRT,
	TITLE,

	MAGIC_MISIL,
	DAMAGE,
	DEAD,
	BROOM,

	MUSHROOM_MOVE,
	SPACE_MOVE,
	CREEEPY_MOVE
};

enum COLLIDER_TYPE
{
	BOUNDING_BOX = 1,
	BOUNDING_SPHERE
};

enum MESH_TYPE_ID
{
	CUBE_MESH,
	TERRAIN_MESH,
	QUADTREE_TERRAIN_MESH,
	CYLINDER_MESH
};

enum SHAPE_TYPE_ID
{
	RECTANGLE_SHAPE,
	BROOMEFFECT_SHAPE
};

enum LIGHT_TYPE
{
	POINT_LIGHT,
	SPOT_LIGHT,
	DIRECTIONAL_LIGHT
};
//// Enum Type ////////////////////////////////////

//// 구현 스킬에 대하여 ////////////////////////////
#define COOLTIME_FIREBALL 3
#define COOLTIME_ICEBALL 3
#define COOLTIME_LIGHTNINGBALL 3
#define COOLTIME_SHIELD 5
#define COOLTIME_BLESSING 5
#define COOLTIME_HEALING 10

#define USEMP_FIREBALL 10
#define USEMP_ICEBALL 10
#define USEMP_LIGHTNINGBALL 10
#define USEMP_SHIELD 5
#define USEMP_BLESSING 15
#define USEMP_HEALING 15 
//// 구현 스킬에 대하여 ////////////////////////////

#define SKILL_TO_CHOOSE 6 // 고를수 있는 스킬 가짓수
#define SKILL_SELECTED 4 // 선택 가능한 스킬 가짓수

#define PI				    3.141592

#define EPSILON				1.0e-10f

#define MOUSE_WHEEL_UP		120
#define MOUSE_WHEEL_DOWN	-120
#define MOUSE_WHEEL_STOP	0

#define CAMERA_FAR			15000.f //1 = 1cm 이므로 7000cm = 70m 

/////////////////////////////////////////////// define 정의  
#define _WITH_DEBUG_TERRAIN_MAX_POS
// #define CHECK_SUBVIEWS
#define CHECK_FRAMERATE // 정의한경우, 프레임 레이트가 상단 타이틀에 보인다.

// #define _WITH_DEBUG_FRAME_HIERARCHY
#define _WITH_DEBUG_SKINNING_BONE
#define _WITH_DISPLAY_TEXTURE_NAME 
/////////////////////////////////////////////// define 정의


//// Root Parameter Index /////////////////////////////////////////// 
#define ROOTPATAMETER_COUNT 28

const UINT ROOTPARAMETER_WORLD = 0;
const UINT ROOTPARAMETER_CAMERA = 1;
const UINT ROOTPARAMETER_MATERIALS = 2;
const UINT ROOTPARAMETER_LIGHTS = 3;
const UINT ROOTPARAMETER_TEXTURE = 4;
const UINT ROOTPARAMETER_TEXTUREBASE = 5;
const UINT ROOTPARAMETER_TEXTUREDETAIL = 6;
const UINT ROOTPARAMETER_ALBEDO = 7;
const UINT ROOTPARAMETER_SPECULAR = 8;
const UINT ROOTPARAMETER_NORMAL = 9;
const UINT ROOTPARAMETER_METALLIC = 10;
const UINT ROOTPARAMETER_SKINNEDBONEOFFSET = 11;
const UINT ROOTPARAMETER_SKINNEDBONETRANSFORM = 12;
const UINT ROOTPARAMETER_EMISSION_3 = 13;
const UINT ROOTPARAMETER_EMISSION_1 = 14;
const UINT ROOTPARAMETER_EMISSION_2 = 15;
const UINT ROOTPARAMETER_PICKINGPOINT = 16;
const UINT ROOTPARAMETER_INSTANCING = 17;
const UINT ROOTPARAMETER_SKYBOX = 18;
const UINT ROOTPARAMETER_HPPERCENTAGE = 19;
const UINT ROOTPARAMETER_TIME = 20;
const UINT ROOTPARAMETER_SHADOWTEXTURE = 21;
const UINT ROOTPARAMETER_LIGHTFORSHADOW = 22;
const UINT ROOTPARAMETER_UAV = 23;
const UINT ROOTPARAMETER_PLAYERSHADOWTEXTURE = 24;
const UINT ROOTPARAMETER_PLAYER_LIGHTFORSHADOW = 25;
const UINT ROOTPARAMETER_AVGLUM = 26; // 최종 휘도값
const UINT ROOTPARAMETER_MIDDLEAVGLUM = 27; // 중간 휘도값


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


//// 조작키 ///////////////////////////////////////////  
#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08 
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
