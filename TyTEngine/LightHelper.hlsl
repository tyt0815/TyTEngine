#include "LightDefine.tyt"

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Position;
	float Range;
	float3 Attenuation;
	float Pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
    float3 Position;
	float Range;
	float3 Direction;
	float Spot;
	float3 Attenuation;
	float Pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};

void ComputeDirectionalLight(
	Material Mat,
	DirectionalLight Lit,
	float3 Normal, float3 ToEye,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular
)
{
	Ambient = float4( 0.f, 0.f, 0.f, 0.f );
	Diffuse = float4( 0.f, 0.f, 0.f, 0.f );
	Specular = float4( 0.f, 0.f, 0.f, 0.f );

	float3 LightVector = -Lit.Direction;

	Ambient = Mat.Ambient * Lit.Ambient;

	float DiffuseFactor = dot(LightVector, Normal);
	[flatten]
	if (DiffuseFactor > 0.f)
	{
		float3 v = reflect(-LightVector, Normal);	// �ݻ������ ����ϴ� �Լ�
		float4 SpecularFactor = pow(max(dot(v, ToEye), 0.f), Mat.Specular.w);

		Diffuse = DiffuseFactor * Mat.Diffuse * Lit.Diffuse;
		Specular = SpecularFactor * Mat.Specular * Lit.Specular;
	}
}

void ComputePointLight(
	Material Mat,
	PointLight Lit,
	float3 Pos, float3 Normal, float3 ToEye,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular
)
{
	Ambient = float4(0.f, 0.f, 0.f, 0.f);
	Diffuse = float4(0.f, 0.f, 0.f, 0.f);
	Specular = float4(0.f, 0.f, 0.f, 0.f);

	float3 LightVector = Lit.Position - Pos;

	float Distance = length(LightVector);

	if (Distance > Lit.Range)
	{
		return;
	}

	LightVector /= Distance;

	Ambient = Mat.Ambient * Lit.Ambient;

	float DiffuseFactor = dot(LightVector, Normal);
	[flatten]
	if (DiffuseFactor > 0.f)
	{
        float3 v = reflect(-LightVector, Normal); // �ݻ������ ����ϴ� �Լ�
        float4 SpecularFactor = pow(max(dot(v, ToEye), 0.f), Mat.Specular.w);

        Diffuse = DiffuseFactor * Mat.Diffuse * Lit.Diffuse;
        Specular = SpecularFactor * Mat.Specular * Lit.Specular;

        float Attenuation = 1.f / dot(Lit.Attenuation, float3(1.f, Distance, Distance * Distance));
        Diffuse *= Attenuation;
        Specular *= Attenuation;
    }
}

void ComputeSpotLight(
	Material Mat,
	SpotLight Lit,
	float3 Pos, float3 Normal, float3 ToEye,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular
)
{
	Ambient = float4(0.f, 0.f, 0.f, 0.f);
	Diffuse = float4(0.f, 0.f, 0.f, 0.f);
	Specular = float4(0.f, 0.f, 0.f, 0.f);
	
	float3 LightVector = Lit.Position - Pos;
	float Distance = length(LightVector);

	if (Distance > Lit.Range)
	{
		return;
	}

	LightVector /= Distance;

	Ambient = Mat.Ambient * Lit.Ambient;

	float DiffuseFactor = dot(LightVector, Normal);
	float Spot = pow(max(dot(-LightVector, Lit.Direction), 0.f), Lit.Spot);
	[flatten]
	if(DiffuseFactor > 0.f)
	{
        float3 v = reflect(-LightVector, Normal);
		float4 SpecularFactor = pow(max(dot(v, ToEye), 0.f), Mat.Specular.w);

		Diffuse = DiffuseFactor * Mat.Diffuse * Lit.Diffuse;
		Specular = SpecularFactor * Mat.Specular * Lit.Specular;

		float Attenuation = Spot / dot(Lit.Attenuation, float3(1.f, Distance, Distance * Distance));
		Diffuse *= Attenuation;
		Specular *= Attenuation;
	}
	Ambient *= Spot;
}