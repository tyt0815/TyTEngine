#pragma once
#include "Actor.h"

class AWater : public AActor
{
public:
	AWater(float Width, float Depth, UINT m, UINT n);
	~AWater();
	virtual void Update(float DeltaTime) override;

};