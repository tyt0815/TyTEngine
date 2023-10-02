#include "LightHelper.hlsl"
#include "MathHelper.hlsl"

cbuffer PerFrame : register(b0)
{
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	float3 gEyePosW;
};

cbuffer PerObject : register(b1)
{
	Material gMaterial;
}

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float4 Color : COLOR;
};

float4 PSMain(VertexOut PIn) : SV_TARGET
{
	PIn.NormalW = normalize(PIn.NormalW);
	float3 ToEyeW = normalize(gEyePosW - PIn.PosW);
	
	float4 Ambient = { 0.f, 0.f, 0.f, 0.f };
	float4 Diffuse = { 0.f, 0.f, 0.f, 0.f };
	float4 Specular = { 0.f, 0.f, 0.f, 0.f };
	
	float4 AmbientOut, DiffuseOut, SpecularOut;
	
	ComputeDirectionalLight(gMaterial, gDirLight, PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
	Ambient += AmbientOut;
	Diffuse += DiffuseOut;
	Specular += SpecularOut;

	ComputePointLight(gMaterial, gPointLight, PIn.PosW, PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
	Ambient += AmbientOut;
	Diffuse += DiffuseOut;
	Specular += SpecularOut;

	float4 LitColor = Ambient + Diffuse + Specular;
	LitColor.a = gMaterial.Diffuse.a;
	return LitColor;
}