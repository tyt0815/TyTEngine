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
	DirectionalLight DirLight,
	float3 Normal, float3 ToEye,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular
)
{
	Ambient = float4( 0.f, 0.f, 0.f, 0.f );
	Diffuse = float4( 0.f, 0.f, 0.f, 0.f );
	Specular = float4( 0.f, 0.f, 0.f, 0.f );

	float3 LightVector = -DirLight.Direction;

	Ambient = Mat.Ambient * DirLight.Ambient;

	float DiffuseFactor = dot(LightVector, Normal);
	if (DiffuseFactor > 0.f)
	{
		float3 v = reflect(-LightVector, Normal);	// 반사방향을 계산하는 함수
		float4 SpecularFactor = pow(max(dot(v, ToEye), 0.f), Mat.Specular.w);

		Diffuse = DiffuseFactor * Mat.Diffuse * DirLight.Diffuse;
		Specular = SpecularFactor * Mat.Specular * DirLight.Specular;
	}
}