#include "LightHelper.hlsl"

Texture2D T2D : register(t0);
SamplerState SmpState : register(s0);

cbuffer PerFrame : register(b0)
{
    uint4 gNumLights; // Dir, Point, Spot, Pad
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

cbuffer Lit : register(b2)
{
    DirectionalLight gDirLit[MAX_DIR_LIT];
    PointLight gPointLit[MAX_POINT_LIT];
    SpotLight gSpotLit[MAX_SPOT_LIT];
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
	
    uint i = 0;
	// Directional Light
    for (i = 0; i < gNumLights.x; ++i)
    {
		ComputeDirectionalLight(gMaterial, gDirLit[i], PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
		Ambient += AmbientOut;
		Diffuse += DiffuseOut;
		Specular += SpecularOut;
    }
	
	// Directional Light
    for (i = 0; i < gNumLights.y; ++i)
    {
        ComputePointLight(gMaterial, gPointLit[i], PIn.PosW, PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
        Ambient += AmbientOut;
        Diffuse += DiffuseOut;
        Specular += SpecularOut;
    }
	
	// Directional Light
    for (i = 0; i < gNumLights.z; ++i)
    {
        ComputeSpotLight(gMaterial, gSpotLit[i], PIn.PosW, PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
        Ambient += AmbientOut;
        Diffuse += DiffuseOut;
        Specular += SpecularOut;
    }
    

	

    float4 LitColor = TexColor * (Ambient + Diffuse) + Specular;
	LitColor.a = gMaterial.Diffuse.a;
	return LitColor;
}