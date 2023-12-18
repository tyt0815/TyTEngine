#pragma once
#include "Object.h"

class AActor : public TObject
{
public:
	AActor(EBlendMode BlendMod = EBlendMode::EBMOpaque);
	~AActor();
	virtual void Update(float DeltaTime) override;
};