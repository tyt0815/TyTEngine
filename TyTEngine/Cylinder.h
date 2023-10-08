#pragma once
#include "Actor.h"

class ACylinder : public AActor
{
public:
	ACylinder();
	~ACylinder();
	virtual void Update(float DeltaTime) override;
};