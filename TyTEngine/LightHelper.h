#pragma once

struct SDirectionalLight
{
	SDirectionalLight();
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad;	//XMFLOAT3 �� ���� 4����Ʈ�� ��� ������ ä���ִ� �뵵�ε�
};

struct SPointLight
{
	SPointLight();
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
	SSpotLight();
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