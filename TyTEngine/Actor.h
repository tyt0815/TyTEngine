#pragma once
#include "Object.h"

class AActor : public TObject
{
public:
	AActor();
	~AActor();
	virtual void Update(float DeltaTime) override;
};