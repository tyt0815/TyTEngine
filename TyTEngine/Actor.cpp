#include "pch.h"
#include "Actor.h"
#include "ObjectManager.h"

AActor::AActor(EBlendMode BlendMod):
	TObject()
{
	switch (BlendMod)
	{
	case TObject::EBlendMode::EBMOpaque:
		TObjectManager::GetInstance()->mOpaqueActors.push_back(unique_ptr<TObject>(this));
		break;
	case TObject::EBlendMode::EBMTranslucent:
		TObjectManager::GetInstance()->mTranslucentActors.push_back(unique_ptr<TObject>(this));
		break;
	default:
		break;
	}
}

AActor::~AActor()
{
	TObject::~TObject();
}

void AActor::Update(float DeltaTime)
{
	TObject::Update(DeltaTime);
}
