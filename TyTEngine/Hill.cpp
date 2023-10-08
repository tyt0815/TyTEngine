#include "pch.h"
#include "Hill.h"

AHill::AHill(float Width, float Depth, UINT m, UINT n) :
	AActor()
{
	UGeometryGenerator::MeshData Mesh;
	UGeometryGenerator::CreateHill(Width, Depth, m, n, Mesh);
	mMaterial.Ambient = {.5f, .5f, .5f, 1.f};
	mMaterial.Diffuse = {1.f, 1.f, 1.f, 1.f};
	mMaterial.Specular = { 0.1f, 0.1f, 0.1f, 0.1f };
	mTexScale = XMFLOAT3(Width / 15.f, Depth / 15.f, 0.f);

	SetD3DBuffers(Mesh);
}

AHill::~AHill()
{
	TObject::~TObject();
}
