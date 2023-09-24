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

	void CreateCubeObject(XMFLOAT3 Scale, XMFLOAT3 Rotation, XMFLOAT3 Location, const XMVECTORF32 Color);

};