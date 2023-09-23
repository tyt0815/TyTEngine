#pragma once

#include "Direct3DApp.h"
#include "CommonUtility.h"

class CCore : public CDirect3DApp
{
public:
	SINGLETON(CCore);
	bool Init(HINSTANCE hInstance, int CmdShow);
	virtual void OnResize() override;
	void UpdateScene(float dt);
	void DrawScene();

	virtual void OnMouseDown(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM BtnState, int x, int y) override;
	HRESULT CreateD3D11Buffer(D3D11_BUFFER_DESC* BufferDesc, D3D11_SUBRESOURCE_DATA* InitData, ID3D11Buffer **Buffer);

private:
	CCore();
	~CCore();
	void BuildGeometryBuffers();
	void BuildMat();
	void BuildShader();
	void BuildVertexLayout();

	POINT mLastMousePos;

	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mWVPBuffer;
	ID3DBlob* mVSBlob;
	ID3D11VertexShader* mVertexShader;
	ID3DBlob* mPSBlob;
	ID3D11PixelShader* mPixelShader;
	
	XMFLOAT4X4 mViewMat;
	XMFLOAT4X4 mProjMat;
	float mRadius;	// Camera�� Look at point���� �Ÿ�
	float mTheta;	// Camera�� w����(look at ���� ����)�� xy��鿡 ���翵�� ���� * -1�� ���Ϳ� x����� ���� 
	float mPhi;		// Camera�� w���Ϳ� Global y����� ����

};