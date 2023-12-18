#include "pch.h"
#include "Cube.h"

ACube::ACube():
	AActor(EBlendMode::EBMTranslucent)
{
	UGeometryGenerator::MeshData Mesh;
	UGeometryGenerator::CreateCube(2, 2, 2, Mesh);
	mMaterial.Ambient = { .5f, .5f, .5f, 1.f };
	mMaterial.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	mMaterial.Specular = { 0.1f, 0.1f, 0.1f, 0.1f };

	SetD3DBuffers(Mesh);
}

ACube::~ACube()
{
	AActor::~AActor();
}

void ACube::Update(float DeltaTime)
{
	AActor::Update(DeltaTime);

	mRotation.y = mRunningTime;
}
