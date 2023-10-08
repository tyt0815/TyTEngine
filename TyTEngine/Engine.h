#pragma once

#include "Direct3DApp.h"
#include "CommonUtility.h"

class TObject;

class TEngine : public TDirect3DApp
{
public:
	SINGLETON(TEngine);
	bool Init(HINSTANCE hInstance, int CmdShow);
	virtual void OnResize() override;
	void UpdateScene(float dt);
	void DrawScene();
	void ObjectsBeginPlay();
	void VSPerObjectConstantBufferUpdate(unique_ptr<TObject>& Object);
	void PSPerFrameConstantBufferUpdate();
	void PSPerObjectConstantBufferUpdate(unique_ptr<TObject>& Object);
	void PSLitConstantBufferUpdate();

	virtual void OnMouseDown(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM BtnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM BtnState, int x, int y) override;


private:
	TEngine();
	~TEngine();
	void BuildConstantBuffer();
	void BuildShader();
	void BuildVertexLayout();
	void CreateObjects();
	void CreateLits();
	void AddLit(SDirectionalLight& DirLit1);
	void AddLit(SPointLight& PointLit1);
	void AddLit(SSpotLight& SpotLit1);

	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mVSPerObjectConstantBuffer;
	ID3D11Buffer* mPSPerFrameConstantBuffer;
	ID3D11Buffer* mPSPerObjectConstantBuffer;
	ID3D11Buffer* mPSLitConstantBuffer;
	ID3DBlob* mVSBlob;
	ID3D11VertexShader* mVertexShader;
	ID3DBlob* mPSBlob;
	ID3D11PixelShader* mPixelShader;
	ID3D11SamplerState* mSamplerState;

	POINT mLastMousePos;
	
	XMFLOAT4X4 mViewMat;
	XMFLOAT4X4 mProjMat;
	XMFLOAT3 mCameraPos;
	float mRadius;	// Camera�� Look at point���� �Ÿ�
	float mTheta;	// Camera�� w����(look at ���� ����)�� xy��鿡 ���翵�� ���� * -1�� ���Ϳ� x����� ���� 
	float mPhi;		// Camera�� w���Ϳ� Global y����� ����

};