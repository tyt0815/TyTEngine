#pragma once
#include "Actor.h"

class ACube : public AActor
{
public:
	ACube();
	~ACube();
	virtual void Update(float DeltaTime) override;
};