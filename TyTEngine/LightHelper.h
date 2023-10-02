#pragma once

struct DirectionalLight
{
	DirectionalLight();
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad;	//XMFLOAT3 를 쓰면 4바이트가 비기 때문에 채워주는 용도인듯
};

struct PointLight
{
	PointLight();
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	
	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Attenuation;
	float Pad;
};

struct SpotLight
{
	SpotLight();
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

struct Material
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;
};