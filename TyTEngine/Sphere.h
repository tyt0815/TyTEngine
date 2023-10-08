#pragma once
#include "Actor.h"

class ASphere : public AActor
{
public:
	ASphere();
	~ASphere();
	virtual void Update(float DeltaTime) override;
	virtual void BeginPlay() override;
private:
	XMFLOAT3 mStartLocation;
};