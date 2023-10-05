#include "LightHelper.hlsl"

Texture2D T2D : register(t0);
SamplerState SmpState : register(s0);

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
    float2 UV : TEXCOORD;
};

float4 PSMain(VertexOut PIn) : SV_TARGET
{	
    float4 TexColor = T2D.Sample(SmpState, PIn.UV);
	
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

	ComputeSpotLight(gMaterial, gSpotLight, PIn.PosW, PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
	Ambient += AmbientOut;
	Diffuse += DiffuseOut;
	Specular += SpecularOut;

    float4 LitColor = TexColor * (Ambient + Diffuse) + Specular;
	LitColor.a = gMaterial.Diffuse.a;
	return LitColor;
}