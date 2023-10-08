#include "pch.h"
#include "ObjectManager.h"

TObjectManager::TObjectManager()
{
	mDirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Direction = UMathHelper::NormalizeXMFLOAT(XMFLOAT3(1.f, -1.f, 0.f));
	mPointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPointLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPointLight.Position = {};
	mPointLight.Range = 15.f;
	mPointLight.Attenuation = XMFLOAT3(1.f, .1f, 0.01f);
	mSpotLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSpotLight.Position = { 3.f, 3.f, 0.f };
	mSpotLight.Range = 50.f;
	mSpotLight.Attenuation = XMFLOAT3(1.f, .1f, 0.01f);
	mSpotLight.Direction = UMathHelper::NormalizeXMFLOAT(XMFLOAT3(-1.f, -1.f, 0.f));
	mSpotLight.Spot = 5.f;
}

TObjectManager::~TObjectManager()
{
}