#pragma once
#include "ParameterForm.h"

class ParameterFormManager
{
	static ParameterFormManager* m_Instance;

private:
	ParameterForm* m_CurrParameterForm{ nullptr };

private:
	ParameterFormManager();
	~ParameterFormManager();

public:
	//static ParameterFormManager* GetInstance()
	//{
	//	if (m_Instance == nullptr)
	//	{
	//		m_Instance = new ParameterFormManager;
	//	}
	//	return m_Instance;
	//}

	//const ParameterForm* GetCurrForm() const { return m_CurrParameterForm; }
	//void SetCurrForm(ParameterForm* pParameterForm)
	//{
	//	m_CurrParameterForm = pParameterForm;
	//}
};