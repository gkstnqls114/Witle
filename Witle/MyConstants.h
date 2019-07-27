#pragma once

// 
// MyConstants.h
// ��ó������ �����ϴ� ��� ����
// 


//// Enum Type ////////////////////////////////////
enum ENUM_SKILL
{
	SKILL_FIREBALL,
	SKILL_ICEBALL,
	SKILL_LIGHTNINGBALL, 
	SKILL_SHIELD,
	SKILL_BLESSING,
	SKILL_HEALING,
	SKILL_METEOR,
	SKILL_ICESPEAR

	// �ִ� 8������... ���̻� ���� X
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


#define SKILL_TO_CHOOSE 8 // ���� �ִ� ��ų ������
#define SKILL_SELECTED 4 // ���� ������ ��ų ������
//// Enum Type ////////////////////////////////////

#define PI				    3.141592

#define EPSILON				1.0e-10f

#define MOUSE_WHEEL_UP		120
#define MOUSE_WHEEL_DOWN	-120
#define MOUSE_WHEEL_STOP	0

#define CAMERA_FAR			15000.f //1 = 1cm �̹Ƿ� 7000cm = 70m 

/////////////////////////////////////////////// define ����  
#define _WITH_DEBUG_TERRAIN_MAX_POS
// #define CHECK_SUBVIEWS
#define CHECK_FRAMERATE // �����Ѱ��, ������ ����Ʈ�� ��� Ÿ��Ʋ�� ���δ�.

// #define _WITH_DEBUG_FRAME_HIERARCHY
#define _WITH_DEBUG_SKINNING_BONE
#define _WITH_DISPLAY_TEXTURE_NAME 
/////////////////////////////////////////////// define ����


//// Root Parameter Index /////////////////////////////////////////// 
#define ROOTPATAMETER_COUNT 25

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
const UINT ROOTPARAMETER_READWRITE = 23;
const UINT ROOTPARAMETER_PLAYERSHADOWTEXTURE = 24;


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
// �� �� �߿� �ϳ��� define
//// Animation Type /////////////////////////////////////////// 


//// ���� �� ���� ///////////////////////////////////////////  
#define MAX_LIGHTS 8
#define MAX_MATERIALS 8
//// ���� �� ���� ///////////////////////////////////////////  


//// HeightMapImage.h ���� define ////////////////////////
#define _WITH_APPROXIMATE_OPPOSITE_CORNER
//// HeightMapImage.h ���� define ////////////////////////


//// ����Ű ///////////////////////////////////////////  
#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08 
//// ����Ű /////////////////////////////////////////// 


//// Load Texture /////////////////////////////////////////// 
#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05
//// Load Texture /////////////////////////////////////////// 


//������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.
#define RANDOM_COLOR XMFLOAT4{rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)}
