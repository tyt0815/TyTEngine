#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Core.h"

CObjectManager::CObjectManager()
{
	CreateCubeObject({ 1,1,1 }, { 0,0,0 }, { 2,0,0 }, Colors::Blue);
	CreateCubeObject({ 1,1,1 }, { 0,0,0 }, { -2,0,0 }, Colors::Red);
}

CObjectManager::~CObjectManager()
{
	mObjects.clear();
}

void CObjectManager::CreateCubeObject(XMFLOAT3 Scale = { 1, 1, 1 }, XMFLOAT3 Rotation = { 0, 0, 0 }, XMFLOAT3 Location = { 0, 0, 0 }, const XMVECTORF32 Color = Colors::Red)
{
	Vertex CubeVertex[] = {
		{ XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT4((const float*)(&Color))	},
		{ XMFLOAT3(-1.f, +1.f, -1.f), XMFLOAT4((const float*)(&Color))   },
		{ XMFLOAT3(+1.f, +1.f, -1.f), XMFLOAT4((const float*)(&Color))   },
		{ XMFLOAT3(+1.f, -1.f, -1.f), XMFLOAT4((const float*)(&Color))  },
		{ XMFLOAT3(-1.f, -1.f, +1.f), XMFLOAT4((const float*)(&Color)) },
		{ XMFLOAT3(-1.f, +1.f, +1.f), XMFLOAT4((const float*)(&Color)) },
		{ XMFLOAT3(+1.f, +1.f, +1.f), XMFLOAT4((const float*)(&Color)) },
		{ XMFLOAT3(+1.f, -1.f, +1.f), XMFLOAT4((const float*)(&Color)) }
	};
	UINT CubeIndex[] = {
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
	mObjects.push_back(make_unique<OObject>(CubeVertex, sizeof(CubeVertex), CubeIndex, sizeof(CubeIndex)));
	mObjects.back()->mScale = Scale;
	mObjects.back()->mRotation = Rotation;
	mObjects.back()->mLocation = Location;
}
