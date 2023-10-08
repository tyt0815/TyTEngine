#include "pch.h"
#include "Sphere.h"

ASphere::ASphere():
	AActor(),
	mStartLocation()
{
	UGeometryGenerator::MeshData Mesh;
	UGeometryGenerator::CreateGeosphere(1, 5, Mesh);
	mMaterial.Ambient = { .5f, .5f, .5f, 1.f };
	mMaterial.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	mMaterial.Specular = { 1.f, 1.f, 1.f, 16.f };
	SetD3DBuffers(Mesh);
}

ASphere::~ASphere()
{
	AActor::~AActor();
}

void ASphere::Update(float DeltaTime)
{
	AActor::Update(DeltaTime);
	mTranslation.y = mStartLocation.y + sinf(mRunningTime);
}

void ASphere::BeginPlay()
{
	mStartLocation = mTranslation;
}
