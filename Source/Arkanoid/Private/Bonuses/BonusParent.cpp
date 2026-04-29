// Alexandr Sharizanov. All rights reserved.


#include "Bonuses/BonusParent.h"


ABonusParent::ABonusParent()
{
	PrimaryActorTick.bCanEverTick = true;

	BonusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus Mesh"));
	SetRootComponent(BonusMesh);
	BonusMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BonusMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BonusMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		BonusMesh->SetStaticMesh(CubeMeshAsset.Object);
	}
}

void ABonusParent::InitScale(const FVector NewScale)
{
	SetActorScale3D(NewScale);
}

void ABonusParent::BeginPlay()
{
	Super::BeginPlay();
	
	Direction = Direction.GetSafeNormal();
}

void ABonusParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAction(DeltaTime);
}

void ABonusParent::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor))
	{
		if (auto Paddle = Cast<APaddle>(OtherActor))
		{
			BonusAction(Paddle);
		}
	}
}

void ABonusParent::MoveAction(const float DeltaTime)
{
	const FVector Offset = Direction * Speed * DeltaTime;
	AddActorWorldOffset(Offset);
}

void ABonusParent::BonusAction(APaddle* Paddle)
{
	Destroy();
}
