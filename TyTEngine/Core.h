#pragma once

#include "Direct3DApp.h"

class CCore : public CDirect3DApp
{
public:
	CCore(HINSTANCE hInstance, int CmdShow);
	~CCore();

	bool Init();
	virtual void OnResize() override;
	void UpdateScene(float dt);
	void DrawScene();

	virtual void OnMouseDown(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM BtnState, int x, int y) override;

private:
	void BuildGeometryBuffers();
	void BuildMat();
	void BuildShader();
	void BuildVertexLayout();

	POINT mLastMousePos;

	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mRectangleVertexBuffers;
	ID3D11Buffer* mRectangleIndexBuffers;
	ID3D11Buffer* mWVPBuffer;
	ID3DBlob* mVSBlob;
	ID3D11VertexShader* mVertexShader;
	ID3DBlob* mPSBlob;
	ID3D11PixelShader* mPixelShader;
	UINT mNumRectangleIndex;
	
	XMFLOAT4X4 mWorldMat;
	XMFLOAT4X4 mViewMat;
	XMFLOAT4X4 mProjMat;
	XMMATRIX mWVPMat;
	float mRadius;	// Camera�� Look at point���� �Ÿ�
	float mTheta;	// Camera�� w����(look at ���� ����)�� xy��鿡 ���翵�� ���� * -1�� ���Ϳ� x����� ���� 
	float mPhi;		// Camera�� w���Ϳ� Global y����� ����

};