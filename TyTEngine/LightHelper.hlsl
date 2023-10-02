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
	float3 Postion;
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
	float4 Specular;	// w는 spec 파워
	float4 Reflect;
};

void ComputeDirectionalLight(
	Material Mat,
	DirectionalLight DirLit,
	float3 Normal, float3 ToEye,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular
)
{
	Ambient = float4( 0.f, 0.f, 0.f, 0.f );
	Diffuse = float4( 0.f, 0.f, 0.f, 0.f );
	Specular = float4( 0.f, 0.f, 0.f, 0.f );

	float3 LightVector = -DirLit.Direction;

	Ambient = Mat.Ambient * DirLit.Ambient;

	float DiffuseFactor = dot(LightVector, Normal);
	[Flatten]
	if (DiffuseFactor > 0.f)
	{
		float3 v = reflect(-LightVector, Normal);	// 반사방향을 계산하는 함수
		float4 SpecularFactor = pow(max(dot(v, ToEye), 0.f), Mat.Specular.w);

		Diffuse = DiffuseFactor * Mat.Diffuse * DirLit.Diffuse;
		Specular = SpecularFactor * Mat.Specular * DirLit.Specular;
	}
}

void ComputePointLight(
	Material Mat,
	PointLight PointLit,
	float3 Pos, float3 Normal, float3 ToEye,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular
)
{
	Ambient = float4(0.f, 0.f, 0.f, 0.f);
	Diffuse = float4(0.f, 0.f, 0.f, 0.f);
	Specular = float4(0.f, 0.f, 0.f, 0.f);

	float3 LightVector = PointLit.Position - Pos;

	float Distance = length(LightVector);

	if (Distance > PointLit.Range)
	{
		return;
	}

	LightVector /= Distance;

	Ambient = Mat.Ambient * PointLit.Ambient;

	float DiffuseFactor = dot(LightVector, Normal);
	[Flatten]
	if (DiffuseFactor > 0.f)
	{
		float3 v = reflect(-LightVector, Normal);	// 반사방향을 계산하는 함수
		float4 SpecularFactor = pow(max(dot(v, ToEye), 0.f), Mat.Specular.w);

		Diffuse = DiffuseFactor * Mat.Diffuse * PointLit.Diffuse;
		Specular = SpecularFactor * Mat.Specular * PointLit.Specular;

		float Attenuation = 1.f / dot(PointLit.Attenuation, float3(1.f, Distance, Distance * Distance));
		Diffuse *= Attenuation;
		Specular *= Attenuation;
	}
}