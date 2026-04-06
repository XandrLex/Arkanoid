// Alexandr Sharizanov. All rights reserved.


#include "World/Block.h"
#include <World/Ball.h>


ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);
}

void ABlock::Init(const FVector NewScale, const int32 LifeAmount, const TSubclassOf<AActor> NewBonusClass)
{
	SetActorScale3D(NewScale);
    BonusClass = NewBonusClass;
}

void ABlock::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (Cast<ABall>(Other))
	{
		if (BonusClass && GetWorld())
		{
			GetWorld()->SpawnActor<AActor>(BonusClass, GetActorLocation(), GetActorRotation());
		}
		
		Destroy();
	}
}