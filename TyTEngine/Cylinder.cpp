#include "pch.h"
#include "Cylinder.h"

ACylinder::ACylinder():
	AActor()
{
	UGeometryGenerator::MeshData Mesh;
	UGeometryGenerator::CreateCylinder(1, 1, 2, 32, 1, Mesh);
	mMaterial.Ambient = { .5f, .5f, .5f, 1.f };
	mMaterial.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	mMaterial.Specular = { 0.1f, 0.1f, 0.1f, 0.1f };
	SetD3DBuffers(Mesh);
}

ACylinder::~ACylinder()
{
	AActor::~AActor();
}

void ACylinder::Update(float DeltaTime)
{
	AActor::Update(DeltaTime);
	mScale = XMFLOAT3(0.5f + 0.5f * sinf(mRunningTime), 0.5f + 0.5f * sinf(mRunningTime), 0.5f + 0.5f * sinf(mRunningTime));
}
