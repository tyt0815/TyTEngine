#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Core.h"

CObjectManager::CObjectManager()
{
	CreateCubeObject({ 1,1,1 }, { 0,0,0 }, { 2,0,0 }, Colors::Blue);
	CreateCylinderObject({ 1,1,1 }, { 0,0,0 }, { -2,0,0 }, Colors::Red, 1, 1, 2, 128, 1);
	CreateGridHillObject({ 1,1,1 }, { 0,0,0 }, { 0,-5,0 });
}

CObjectManager::~CObjectManager()
{
	mObjects.clear();
}

void CObjectManager::CreateCubeObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location, const XMVECTORF32 Color)
{
	vector<Vertex> CubeVertex = {
		{ XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT4((const float*)(&Color))	},
		{ XMFLOAT3(-1.f, +1.f, -1.f), XMFLOAT4((const float*)(&Color))   },
		{ XMFLOAT3(+1.f, +1.f, -1.f), XMFLOAT4((const float*)(&Color))   },
		{ XMFLOAT3(+1.f, -1.f, -1.f), XMFLOAT4((const float*)(&Color))  },
		{ XMFLOAT3(-1.f, -1.f, +1.f), XMFLOAT4((const float*)(&Color)) },
		{ XMFLOAT3(-1.f, +1.f, +1.f), XMFLOAT4((const float*)(&Color)) },
		{ XMFLOAT3(+1.f, +1.f, +1.f), XMFLOAT4((const float*)(&Color)) },
		{ XMFLOAT3(+1.f, -1.f, +1.f), XMFLOAT4((const float*)(&Color)) }
	};
	vector<UINT> CubeIndex = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	PushObjectBuffers(CubeVertex, CubeIndex, Scale, Rotation, Location);
}

void CObjectManager::PushObjectBuffers(std::vector<Vertex> Vertices, std::vector<UINT> Indices, const DirectX::XMFLOAT3 Scale, const DirectX::XMFLOAT3 Rotation, const DirectX::XMFLOAT3 Location)
{
	mObjects.push_back(make_unique<OObject>(Vertices, sizeof(Vertex) * Vertices.size(), Indices, sizeof(UINT) * Indices.size()));
	mObjects.back()->mScale = Scale;
	mObjects.back()->mRotation = Rotation;
	mObjects.back()->mLocation = Location;
}

void CObjectManager::CreateGridHillObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location)
{
	OGeometryGenerator::MeshData Grid;
	OGeometryGenerator::CreateGrid(500.f, 500.f, 50, 50, Grid);
	vector<Vertex> GridVertices(Grid.Vertices.size());
	for (size_t i = 0; i < Grid.Vertices.size(); ++i)
	{
		XMFLOAT3 Pos = Grid.Vertices[i].Position;
		Pos.y = GetHeight(Pos.x, Pos.z);
		GridVertices[i].Pos = Pos;
		if (Pos.y < -10.0f)
		{
			// Sandy beach color.
			GridVertices[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		}
		else if (Pos.y < 5.0f)
		{
			// Light yellow-green.
			GridVertices[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (Pos.y < 12.0f)
		{
			// Dark yellow-green.
			GridVertices[i].Color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (Pos.y < 20.0f)
		{
			// Dark brown.
			GridVertices[i].Color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			// White snow.
			GridVertices[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	PushObjectBuffers(GridVertices, Grid.Indices, Scale, Rotation, Location);
}

void CObjectManager::CreateCylinderObject(
	const XMFLOAT3 Scale,
	const XMFLOAT3 Rotation,
	const XMFLOAT3 Location,
	const XMVECTORF32 Color,
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
		CylinderVertices[i] = { Cylinder.Vertices[i].Position, XMFLOAT4((const float*)(&Color)) };
	}
	PushObjectBuffers(CylinderVertices, Cylinder.Indices, Scale, Rotation, Location);
}

float CObjectManager::GetHeight(float x, float z)
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
