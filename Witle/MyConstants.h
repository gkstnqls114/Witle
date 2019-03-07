#pragma once

/////////////////////////////////////////////// ��ó������ ���� ���� 
/////////////////////////////////////////////// ��ó������ ���� ����

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

#define EPSILON				1.0e-10f

#define G_BUFFER_ROOT_PARMATER 0
#define COMPUTE_UAV_ROOT_PARAMETER 0
#define COMPUTE_SRV_ROOT_PARAMETER G_BUFFER_ROOT_PARMATER + 3

#define NUM32BITTOBYTE 4

/////////////////////////////////////////////// define ����
#define CHECK_ROOT_SIGNATURE 
#define CHECK_TERRAIN_MAX_POS
#define CHECK_ANOTHER_CAMERA
#define CHECK_G_BUFFERS
#define CHECK_CONSOLE_TEST
#define CHECK_FRAMERATE // �����Ѱ��, ������ ����Ʈ�� ��� Ÿ��Ʋ�� ���δ�.
// #define _DEBUG_FRAME_HIERARCHY
/////////////////////////////////////////////// define ����


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
////  Mesh File Load ///////////////////////////////////////////


/////////////////////////////////////////////// Material File Load
#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40
/////////////////////////////////////////////// Material File Load


/////////////////////////////////////////////// ���� 
#define MAX_LIGHTS 8
#define MAX_MATERIALS 8
/////////////////////////////////////////////// ����


/////////////////////////////////////////////// ����Ű
#define DIR_FORWARD 0x01
#define DIR_BACKWARD 0x02
#define DIR_LEFT 0x04
#define DIR_RIGHT 0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20
/////////////////////////////////////////////// ����Ű

#define BILLBOARD_PITCH_X 700.F
#define BILLBOARD_PITCH_Y 100.F
#define BILLBOARD_PITCH_Z 700.F

#define BILLBOARD_WIDTH 500.F
#define BILLBOARD_HEIGHT 500.F

//#define _WITH_TEXT_MESH
//#define _WITH_BIN_MESH
#define _WITH_OBJ_MESH

#define MOUSE_WHEEL_UP		120
#define MOUSE_WHEEL_DOWN	-120
#define MOUSE_WHEEL_STOP	0


#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05


#define CAMERA_FAR			50000.F 

//������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.
#define RANDOM_COLOR XMFLOAT4{rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)}
/////////////////////////////////////////////// define ����
