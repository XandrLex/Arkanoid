// Alexandr Sharizanov. All rights reserved.


#include "World/Block.h"
#include <World/Ball.h>


ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);
	LifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("Life Component"));
}

void ABlock::Init(const FVector NewScale, const int32 LifeAmount, const TSubclassOf<AActor> NewBonusClass)
{
	SetActorScale3D(NewScale);
    BonusClass = NewBonusClass;
	LifeComponent->SetLife(LifeAmount);
}

void ABlock::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (const auto Ball = Cast<ABall>(Other))
	{
		if (IsValid(LifeComponent))
		{
			LifeComponent->TakeDamage(Ball->GetPower());
			if (!LifeComponent->IsAlive())
			{
				if (BonusClass && GetWorld())
				{
					GetWorld()->SpawnActor<AActor>(BonusClass, GetActorLocation(), GetActorRotation());
				}
				Destroy();
			}
			else
			{
				if (LifeMaterials.IsValidIndex(LifeComponent->GetLife() - 1))
					StaticMesh->SetMaterial(0, LifeMaterials[LifeComponent->GetLife() - 1]);
			}
		}
	}
}

void ABlock::BeginPlay()
{
	Super::BeginPlay();

	if (LifeMaterials.IsValidIndex(LifeComponent->GetLife() - 1))
		StaticMesh->SetMaterial(0, LifeMaterials[LifeComponent->GetLife() - 1]);
}
