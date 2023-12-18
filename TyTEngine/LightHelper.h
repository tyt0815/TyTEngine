#pragma once
#include "LightDefine.tyt"

struct SDirectionalLight
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad;	//XMFLOAT3 를 쓰면 4바이트가 비기 때문에 채워주는 용도인듯
};

struct SPointLight
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	
	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Attenuation;
	float Pad;
};

struct SSpotLight
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	XMFLOAT3 Position;
	float Range;
	
	XMFLOAT3 Direction;
	float Spot;

	XMFLOAT3 Attenuation;
	float Pad;
};

struct SMaterial
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;
};