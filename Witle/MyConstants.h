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
#define CHECK_CONSOLE_TEST
#define CHECK_FRAMERATE // �����Ѱ��, ������ ����Ʈ�� Ÿ��Ʋ ȭ�鿡 ���δ�.
/////////////////////////////////////////////// define ����


/////////////////////////////////////////////// �йи� ���̵� ����

/////////////////////////////////////////////// �йи� ���̵� ����


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
