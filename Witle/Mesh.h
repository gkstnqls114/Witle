#pragma once
#include "ComponentBase.h"

// Mesh Class
// 모델 안에 애니메이션 정보(정점 안에 들어가는 뼈대에 대한 정보)가 들어가지 않습니다.
class Mesh :
	public ComponentBase
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void Create() = 0;
	virtual void Init() = 0;
	
};

