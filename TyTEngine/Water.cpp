#include "pch.h"
#include "Water.h"

AWater::AWater(float Width, float Depth, UINT m, UINT n) :
	AActor(EBlendMode::EBMTranslucent)
{
	UGeometryGenerator::MeshData Mesh;
	UGeometryGenerator::CreateWater(Width, Depth, m, n, Mesh);
	mTexScale = XMFLOAT3(Width / 15.f, Depth / 15.f, 0.f);
	mMaterial.Ambient = { .5f, .5f, .5f, 1.f };
	mMaterial.Diffuse = { 1.f, 1.f, 1.f, 0.3f };
	mMaterial.Specular = { 0.1f, 0.1f, 0.1f, 0.1f };

	SetD3DBuffers(Mesh);
}

AWater::~AWater()
{
	AActor::~AActor();
}

void AWater::Update(float DeltaTime)
{
	AActor::Update(DeltaTime);

	mTexTranslation = XMFLOAT3(mRunningTime * 0.5f, mRunningTime * 0.5f, 0.f);
}
