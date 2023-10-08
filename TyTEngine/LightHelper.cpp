#include "pch.h"
#include "LightHelper.h"

SDirectionalLight::SDirectionalLight()
{
	ZeroMemory(this, sizeof(this));
}

SPointLight::SPointLight()
{
	ZeroMemory(this, sizeof(this));
}

SSpotLight::SSpotLight()
{
	ZeroMemory(this, sizeof(this));
}
