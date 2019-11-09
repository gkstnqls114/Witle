#include "stdafx.h"
#include "PostprocessingSetting.h"


const bool PostprocessingSetting::DefferedRendering{ false };
bool PostprocessingSetting::isBloom{ false };
bool PostprocessingSetting::isToneCurve{ false };

void PostprocessingSetting::ActiveBloom()
{
	isBloom = true;
}

void PostprocessingSetting::DeactiveBloom()
{
	isBloom = false;
}

void PostprocessingSetting::ActiveTonecurve()
{
	isToneCurve = true;
}

void PostprocessingSetting::DeactiveTonecurve()
{
	isToneCurve = false;
}
