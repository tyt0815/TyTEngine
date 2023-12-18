#include "LightHelper.hlsl"

Texture2D T2D : register(t0);
SamplerState SmpState : register(s0);

cbuffer PerFrame : register(b0)
{
    uint4 gNumLights; // Dir, Point, Spot, Pad
	float3 gEyePosW;
    float gPad;
    float4 gFogColor;
    float gFogStart;
    float gFogRange;
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

void ComputePong(
    VertexOut PIn,
    float3 ToEyeW,
    Material Mat,
    out float4 Ambient,
    out float4 Diffuse,
    out float4 Specular
)
{
    
    float4 AmbientOut, DiffuseOut, SpecularOut;
    uint i = 0;
	// Directional Light
    for (i = 0; i < gNumLights.x; ++i)
    {
        ComputeDirectionalLight(Mat, gDirLit[i], PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
        Ambient += AmbientOut;
        Diffuse += DiffuseOut;
        Specular += SpecularOut;
    }
	
	// Directional Light
    for (i = 0; i < gNumLights.y; ++i)
    {
        ComputePointLight(Mat, gPointLit[i], PIn.PosW, PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
        Ambient += AmbientOut;
        Diffuse += DiffuseOut;
        Specular += SpecularOut;
    }
	
	// Directional Light
    for (i = 0; i < gNumLights.z; ++i)
    {
        ComputeSpotLight(Mat, gSpotLit[i], PIn.PosW, PIn.NormalW, ToEyeW, AmbientOut, DiffuseOut, SpecularOut);
        Ambient += AmbientOut;
        Diffuse += DiffuseOut;
        Specular += SpecularOut;
    }
}


float4 PSMain(VertexOut PIn) : SV_TARGET
{	
    float4 TexColor = T2D.Sample(SmpState, PIn.UV);
	
	PIn.NormalW = normalize(PIn.NormalW);
	float3 ToEyeW = gEyePosW.xyz - PIn.PosW;
    float DistanceToEye = length(ToEyeW);
    ToEyeW /= DistanceToEye;
	
	float4 Ambient = { 0.f, 0.f, 0.f, 0.f };
	float4 Diffuse = { 0.f, 0.f, 0.f, 0.f };
	float4 Specular = { 0.f, 0.f, 0.f, 0.f };
	
    clip(TexColor.a - 0.1f);
	
    ComputePong(PIn, ToEyeW, gMaterial, Ambient, Diffuse, Specular);
    
    float4 LitColor = TexColor * (Ambient + Diffuse) + Specular;
    
    float FogLerp = saturate((DistanceToEye - gFogStart) / gFogRange);
    LitColor = lerp(LitColor, gFogColor, FogLerp);
    
	LitColor.a = gMaterial.Diffuse.a * TexColor.a;
	return LitColor;
}

float4 ShadowMain(VertexOut PIn) : SV_TARGET
{	
    //PIn.NormalW = normalize(PIn.NormalW);
    //float3 ToEyeW = gEyePosW.xyz - PIn.PosW;
    //float DistanceToEye = length(ToEyeW);
    //ToEyeW /= DistanceToEye;
	
    //float4 Ambient = { 0.f, 0.f, 0.f, 0.f };
    //float4 Diffuse = { 0.f, 0.f, 0.f, 0.f };
    //float4 Specular = { 0.f, 0.f, 0.f, 0.f };
    
    
    //Material ShadowMat;
    //ShadowMat.Ambient = (0.0f, 0.0f, 0.0f, 1.0f);
    //ShadowMat.Diffuse = (0.0f, 0.0f, 0.0f, 0.5f);
    //ShadowMat.Specular = (0.0f, 0.0f, 0.0f, 16.0f);
	
    //ComputePong(PIn, ToEyeW, ShadowMat, Ambient, Diffuse, Specular);
    
    //float4 LitColor = ShadowMat.Ambient + ShadowMat.Diffuse + ShadowMat.Specular;
    
    //float FogLerp = saturate((DistanceToEye - gFogStart) / gFogRange);
    //LitColor = lerp(LitColor, gFogColor, FogLerp);
    
    //LitColor.a = gMaterial.Diffuse.a;
    return (0.05f, 0.05f, 0.05f, 0.5f);
}
