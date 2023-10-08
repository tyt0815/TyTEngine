#pragma once
#define MAX_DIR_LIT 3
#define MAX_POINT_LIT 20
#define MAX_SPOT_LIT 20

struct SDirectionalLight
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad;	//XMFLOAT3 �� ���� 4����Ʈ�� ��� ������ ä���ִ� �뵵�ε�
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