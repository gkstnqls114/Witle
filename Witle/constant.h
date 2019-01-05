#pragma once

/////////////////////////////////////////////// 전처리문을 위한 정의
// #define SHOW_G_BUFFERS // 정의한경우, G Buffer를 볼 수 있따.
#define SHOW_FRAMERATE // 정의한경우, 프레임 레이트가 타이틀 화면에 보인다.
#define CONSOLE_TEST
/////////////////////////////////////////////// 전처리문을 위한 정의

enum MESH_TYPE_ID
{
	CUBE_MESH
};

#define EPSILON				1.0e-10f

#define G_BUFFER_ROOT_PARMATER 0
#define COMPUTE_UAV_ROOT_PARAMETER 0
#define COMPUTE_SRV_ROOT_PARAMETER G_BUFFER_ROOT_PARMATER + 3

/////////////////////////////////////////////// define 정의

#define FRAME_BUFFER_WIDTH	800 //1920 / 2 (기준)
#define FRAME_BUFFER_HEIGHT 600//1080 /2  (기준)

// G Buffer가 현재 게임에서 보이는 사이즈
#define G_BUFFER_WINDOW_WIDTH 240 //960 / 4 (기준)
#define G_BUFFER_WINDOW_HEIGHT 135 //1080 (기준)

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

//정점의 색상을 무작위로(Random) 설정하기 위해 사용한다. 각 정점의 색상은 난수(Random Number)를 생성하여 지정한다.
#define RANDOM_COLOR XMFLOAT4{rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)}
/////////////////////////////////////////////// define 정의
