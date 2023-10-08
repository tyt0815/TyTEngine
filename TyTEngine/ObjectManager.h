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
	vector<unique_ptr<SDirectionalLight>> mDirLits;
	vector<unique_ptr<SPointLight>> mPointLits;
	vector<unique_ptr<SSpotLight>> mSpotLits;

private:
	TObjectManager();
	~TObjectManager();

public:
};