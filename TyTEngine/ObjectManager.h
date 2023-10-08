#pragma once
#include "Object.h"
#include "Actor.h"
#include "Cube.h"
#include "Hill.h"
#include "Water.h"
#include "Cylinder.h"
#include "Sphere.h"

class TObjectManager
{
public:
	SINGLETON(TObjectManager);

	vector<unique_ptr<TObject>> mObjects;
	SDirectionalLight mDirLight;
	SPointLight mPointLight;
	SSpotLight mSpotLight;

private:
	TObjectManager();
	~TObjectManager();
};