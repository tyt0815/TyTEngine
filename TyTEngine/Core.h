#pragma once

#include "Direct3DApp.h"

class CCore : public CDirect3DApp
{
public:
	CCore(HINSTANCE hInstance, int CmdShow);
	~CCore();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
};