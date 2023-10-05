#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Core.h"

CObjectManager::CObjectManager()
{
	CreateCubeObject({ 1,1,1 }, { 0,0,0 }, { 2,0,0 });
	CreateCylinderObject({ 1,1,1 }, { 0,0,0 }, { -2,0,0 }, 1, 1, 2, 128, 1);
	CreateGeoSphereObject({ 1,1,1 }, { 0,0,0 }, { 0,2,0 }, 1, 5);
	CreateGridHillObject({ 1, 1, 1 }, { 0, 0, 0 }, { 0,-5,0 });

	mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Direction = NormalizeXMFLOAT3(XMFLOAT3(1.f, -1.f, 0.f));
	mPointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPointLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPointLight.Position = {};
	mPointLight.Range = 15.f;
	mPointLight.Attenuation = XMFLOAT3(1.f, .1f, 0.01f);
	mSpotLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSpotLight.Position = {3.f, 3.f, 0.f};
	mSpotLight.Range = 50.f;
	mSpotLight.Attenuation = XMFLOAT3(1.f, .1f, 0.01f);
	mSpotLight.Direction = NormalizeXMFLOAT3(XMFLOAT3(-1.f, -1.f, 0.f));
	mSpotLight.Spot = 16.f;
}

CObjectManager::~CObjectManager()
{
	mObjects.clear();
}

void CObjectManager::CreateCubeObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location)
{

	OGeometryGenerator::MeshData Cube;
	OGeometryGenerator::CreateCube(2, 2, 2, Cube);
	vector<Vertex> CubeVertices(Cube.Vertices.size());
	for (size_t i = 0; i < Cube.Vertices.size(); ++i)
	{
		CubeVertices[i].Pos = Cube.Vertices[i].Position;
		CubeVertices[i].Normal = Cube.Vertices[i].Normal;
		CubeVertices[i].UV = Cube.Vertices[i].TexC;
	}
	PushObjectBuffers(CubeVertices, Cube.Indices, Scale, Rotation, Location, L"Textures/WoodCrate01.dds");
}

void CObjectManager::CreateGridHillObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location)
{
	OGeometryGenerator::MeshData Grid;
	OGeometryGenerator::CreateGrid(500.f, 500.f, 50, 50, Grid);
	vector<Vertex> GridVertices(Grid.Vertices.size());
	for (size_t i = 0; i < Grid.Vertices.size(); ++i)
	{
		XMFLOAT3 Pos = Grid.Vertices[i].Position;
		Pos.y = GetHillHeight(Pos.x, Pos.z);
		XMFLOAT3 Normal = GetHilNormal(Pos.x, Pos.y);
		GridVertices[i].Pos = Pos;
		GridVertices[i].Normal = Normal;
		GridVertices[i].UV = Grid.Vertices[i].TexC;
	}
	PushObjectBuffers(GridVertices, Grid.Indices, Scale, Rotation, Location, L"Textures/grass.dds");
}

void CObjectManager::CreateCylinderObject(
	const XMFLOAT3 Scale,
	const XMFLOAT3 Rotation,
	const XMFLOAT3 Location,
	float BottomRadius,
	float TopRadius,
	float Height,
	UINT SliceCount,
	UINT StackCount
)
{
	OGeometryGenerator::MeshData Cylinder;
	OGeometryGenerator::CreateCylinder(
		BottomRadius,
		TopRadius,
		Height,
		SliceCount,
		StackCount,
		Cylinder
	);
	vector<Vertex> CylinderVertices;
	CylinderVertices.resize(Cylinder.Vertices.size());
	for (int i = 0; i < Cylinder.Vertices.size(); ++i)
	{
		CylinderVertices[i].Pos = Cylinder.Vertices[i].Position;
		CylinderVertices[i].Normal = Cylinder.Vertices[i].Normal;
		CylinderVertices[i].UV = Cylinder.Vertices[i].TexC;
	}
	PushObjectBuffers(CylinderVertices, Cylinder.Indices, Scale, Rotation, Location, L"Textures/darkbrickdxt1.dds");
}

void CObjectManager::CreateGeoSphereObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location, float Radius, UINT NumSubdivisions)
{
	OGeometryGenerator::MeshData Sphere;
	OGeometryGenerator::CreateGeosphere(Radius, NumSubdivisions, Sphere);
	vector<Vertex> SphereVertices;
	SphereVertices.resize(Sphere.Vertices.size());
	for (int i = 0; i < Sphere.Vertices.size(); ++i)
	{
		SphereVertices[i].Pos = Sphere.Vertices[i].Position;
		SphereVertices[i].Normal = Sphere.Vertices[i].Normal;
		SphereVertices[i].UV = Sphere.Vertices[i].TexC;
	}
	PushObjectBuffers(SphereVertices, Sphere.Indices, Scale, Rotation, Location, L"Textures/moon.dds");
}

void CObjectManager::PushObjectBuffers(
	std::vector<Vertex> Vertices, std::vector<UINT> Indices,
	const DirectX::XMFLOAT3 Scale, const DirectX::XMFLOAT3 Rotation, const DirectX::XMFLOAT3 Location,
	const WCHAR* TextureFileName
)
{
	mObjects.push_back(make_unique<OObject>(Vertices, sizeof(Vertex) * Vertices.size(), Indices, sizeof(UINT) * Indices.size(), TextureFileName));
	mObjects.back()->mScale = Scale;
	mObjects.back()->mRotation = Rotation;
	mObjects.back()->mLocation = Location;
}

float CObjectManager::GetHillHeight(float x, float z)
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

XMFLOAT3 CObjectManager::GetHilNormal(float x, float z)
{
	XMFLOAT3 Normal = XMFLOAT3(
		-0.3f * (0.1f * z * cosf(0.1f * x) + cosf(0.1f * z)),
		1,
		-0.3f * (sinf(0.1f * x) - 0.1f * x * sinf(0.1f * z))
	);
	
	return NormalizeXMFLOAT3(Normal);
}

XMFLOAT3 CObjectManager::NormalizeXMFLOAT3(XMFLOAT3 Input)
{
	XMFLOAT3 Normal;
	XMVECTOR UnitNorm = XMVector3Normalize(XMLoadFloat3(&Input));
	XMStoreFloat3(&Normal, UnitNorm);
	return Normal;
}

XMFLOAT4 CObjectManager::NormalizeXMFLOAT4(XMFLOAT4 Input)
{
	XMFLOAT4 Normal;
	XMVECTOR UnitNorm = XMVector4Normalize(XMLoadFloat4(&Input));
	XMStoreFloat4(&Normal, UnitNorm);
	return Normal;
}
