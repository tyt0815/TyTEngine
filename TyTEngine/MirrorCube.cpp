#include "pch.h"
#include "MirrorCube.h"
#include "ObjectManager.h"

AMirrorCube::AMirrorCube() :
	TObject()
{
	UGeometryGenerator::MeshData Mesh;
	UGeometryGenerator::CreateCube(2, 2, 2, Mesh);
	mMaterial.Ambient = { .5f, .5f, .5f, 1.f };
	mMaterial.Diffuse = { 1.f, 1.f, 1.f, 0.5f };
	mMaterial.Specular = { 0.1f, 0.1f, 0.1f, 0.1f };

	SetD3DBuffers(Mesh);
	TObjectManager::GetInstance()->mMirrorActors.push_back(unique_ptr<TObject>(this));
}

AMirrorCube::~AMirrorCube()
{
	TObject::~TObject();
}
