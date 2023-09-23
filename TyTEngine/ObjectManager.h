#pragma once
#include <queue>

class OObject;

using std::vector;

class CObjectManager
{
public:
	SINGLETON(CObjectManager);

	vector<OObject*> mObjects;

private:
	CObjectManager();
	~CObjectManager();

};