#pragma once
#include "ComponentBase.h"

// Mesh Class
// �� �ȿ� �ִϸ��̼� ����(���� �ȿ� ���� ���뿡 ���� ����)�� ���� �ʽ��ϴ�.
class Mesh :
	public ComponentBase
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void Create() = 0;
	virtual void Init() = 0;
	
protected:
	
};

