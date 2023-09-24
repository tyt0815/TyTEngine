#pragma once
class OObject;

class CObjectManager
{
public:
	SINGLETON(CObjectManager);

	vector<unique_ptr<OObject>> mObjects;

private:
	CObjectManager();
	~CObjectManager();

};