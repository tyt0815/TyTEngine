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

private:
	void BuildGeometryBuffers();
	void BuildShader();
	void BuildVertexLayout();

	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxIB;
	ID3DBlob* mVSBlob;
	ID3D11VertexShader* mVertexShader;
	ID3DBlob* mPSBlob;
	ID3D11PixelShader* mPixelShader;
	UINT mNumBoxIndex;
	
	XMFLOAT4X4 mWorldMat;
	XMFLOAT4X4 mViewMat;
	XMFLOAT4X4 mProjMat;
	float mRadius;	// Camera와 Look at point와의 거리
	float mTheta;	// Camera의 w벡터(look at 방향 벡터)를 xy평면에 정사영한 벡터 * -1한 벡터와 x축과의 각도 
	float mPhi;		// Camera의 w벡터와 Global y축과의 각도

};