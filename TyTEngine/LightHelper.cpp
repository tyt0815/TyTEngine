#include "pch.h"
#include "LightHelper.h"

DirectionalLight::DirectionalLight()
{
	ZeroMemory(this, sizeof(this));
}

PointLight::PointLight()
{
	ZeroMemory(this, sizeof(this));
}

SpotLight::SpotLight()
{
	ZeroMemory(this, sizeof(this));
}
