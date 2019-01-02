#pragma once

/////////////////////////////////////////////// ��ó������ ���� ����
// #define SHOW_G_BUFFERS // �����Ѱ��, G Buffer�� �� �� �ֵ�.
#define SHOW_FRAMERATE // �����Ѱ��, ������ ����Ʈ�� Ÿ��Ʋ ȭ�鿡 ���δ�.
#define CONSOLE_TEST
/////////////////////////////////////////////// ��ó������ ���� ����

#define EPSILON				1.0e-10f

#define G_BUFFER_ROOT_PARMATER 0
#define COMPUTE_UAV_ROOT_PARAMETER 0
#define COMPUTE_SRV_ROOT_PARAMETER G_BUFFER_ROOT_PARMATER + 3

/////////////////////////////////////////////// define ����

#define FRAME_BUFFER_WIDTH	800 //1920 / 2 (����)
#define FRAME_BUFFER_HEIGHT 600//1080 /2  (����)

// G Buffer�� ���� ���ӿ��� ���̴� ������
#define G_BUFFER_WINDOW_WIDTH 240 //960 / 4 (����)
#define G_BUFFER_WINDOW_HEIGHT 135 //1080 (����)

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

#define PLAYER_START_POSITION_Z float(-450.F)

//������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.
#define RANDOM_COLOR XMFLOAT4{rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)}
/////////////////////////////////////////////// define ����
