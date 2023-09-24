#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Core.h"

CObjectManager::CObjectManager()
{
	Vertex CubeVertex[] = {
		{ XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT4((const float*)(&Colors::White))	},
		{ XMFLOAT3(-1.f, +1.f, -1.f), XMFLOAT4((const float*)(&Colors::Black))   },
		{ XMFLOAT3(+1.f, +1.f, -1.f), XMFLOAT4((const float*)(&Colors::Red))   },
		{ XMFLOAT3(+1.f, -1.f, -1.f), XMFLOAT4((const float*)(&Colors::Green))  },
		{ XMFLOAT3(-1.f, -1.f, +1.f), XMFLOAT4((const float*)(&Colors::Blue)) },
		{ XMFLOAT3(-1.f, +1.f, +1.f), XMFLOAT4((const float*)(&Colors::Yellow)) },
		{ XMFLOAT3(+1.f, +1.f, +1.f), XMFLOAT4((const float*)(&Colors::Cyan)) },
		{ XMFLOAT3(+1.f, -1.f, +1.f), XMFLOAT4((const float*)(&Colors::Magenta)) }
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

	//unique_ptr<OObject> Cube = make_unique<OObject>(CubeVertex, sizeof(CubeVertex), CubeIndex, sizeof(CubeIndex));
	//Cube->mLocation = { 2.f, 0.f, 0.f };
	//mObjects.push_back(Cube);
	//unique_ptr<OObject> Cube2 = make_unique<OObject>(CubeVertex, sizeof(CubeVertex), CubeIndex, sizeof(CubeIndex));
	//Cube2->mLocation = { -2.f, 0.f, 0.f };
	//mObjects.push_back(Cube2);
	mObjects.push_back(make_unique<OObject>(CubeVertex, sizeof(CubeVertex), CubeIndex, sizeof(CubeIndex)));
}

CObjectManager::~CObjectManager()
{
	mObjects.clear();
}
