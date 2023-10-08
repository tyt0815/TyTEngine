#include "pch.h"
#include "Actor.h"

AActor::AActor():
	TObject()
{
	
}

AActor::~AActor()
{
	TObject::~TObject();
}

void AActor::Update(float DeltaTime)
{
	TObject::Update(DeltaTime);
}
