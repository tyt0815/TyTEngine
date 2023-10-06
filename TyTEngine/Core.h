#pragma once

#include "Direct3DApp.h"
#include "CommonUtility.h"
//#include "Object.h"
class OObject;

class CCore : public CDirect3DApp
{
public:
	SINGLETON(CCore);
	bool Init(HINSTANCE hInstance, int CmdShow);
	virtual void OnResize() override;
	void UpdateScene(float dt);
	void DrawScene();
	void VSPerObjectConstantBufferUpdate(unique_ptr<OObject>& Object);
	void PSPerFrameConstantBufferUpdate();
	void PSPerObjectConstantBufferUpdate(unique_ptr<OObject>& Object);

	virtual void OnMouseDown(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM BtnState, int x, int y) override;


private:
	CCore();
	~CCore();
	void BuildConstantBuffer();
	void BuildShader();
	void BuildVertexLayout();

	POINT mLastMousePos;

	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mVSPerObjectConstantBuffer;
	ID3D11Buffer* mPSPerFrameConstantBuffer;
	ID3D11Buffer* mPSPerObjectConstantBuffer;
	ID3DBlob* mVSBlob;
	ID3D11VertexShader* mVertexShader;
	ID3DBlob* mPSBlob;
	ID3D11PixelShader* mPixelShader;
	ID3D11SamplerState* mSamplerState;

	
	
	XMFLOAT4X4 mViewMat;
	XMFLOAT4X4 mProjMat;
	XMFLOAT3 mCameraPos;
	float mRadius;	// Camera�� Look at point���� �Ÿ�
	float mTheta;	// Camera�� w����(look at ���� ����)�� xy��鿡 ���翵�� ���� * -1�� ���Ϳ� x����� ���� 
	float mPhi;		// Camera�� w���Ϳ� Global y����� ����

};