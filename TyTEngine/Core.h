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
	float mRadius;	// Camera�� Look at point���� �Ÿ�
	float mTheta;	// Camera�� w����(look at ���� ����)�� xy��鿡 ���翵�� ���� * -1�� ���Ϳ� x����� ���� 
	float mPhi;		// Camera�� w���Ϳ� Global y����� ����

};