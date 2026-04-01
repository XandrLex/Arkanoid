// Alexandr Sharizanov. All rights reserved.


#include "World/Ball.h"
#include "Components/ArrowComponent.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Arrow"));
	ForwardArrow->SetupAttachment(StaticMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(SphereMeshAsset.Object);
	}
}


void ABall::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector(InitParameters.Scale));
	Power = InitParameters.Power;
	Speed = InitParameters.Speed;
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	Direction = GetActorForwardVector().GetSafeNormal();
	SetBallState(EState::Moving);
}


void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EState::Idle:
		break;
	case EState::Moving:
		Move(DeltaTime);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown ball state!"));
		break;
	}
}

void ABall::Move(const float DeltaTime)
{
	const FVector Offset = Direction * Speed * DeltaTime;
	FHitResult HitResult;
	AddActorWorldOffset(Offset, true, &HitResult);
	if(HitResult.bBlockingHit)
	{
		Direction = Direction - 2 * (FVector::DotProduct(Direction, HitResult.Normal)) * HitResult.Normal;
		Direction.Z = 0.0f;
		Direction = Direction.GetSafeNormal();
		if (Speed < InitParameters.MaxSpeed)
		{
			Speed += InitParameters.Speed * 0.1f;
			Speed = FMath::Min(Speed, InitParameters.MaxSpeed);
		}
		UE_LOG(LogTemp, Warning, TEXT("Ball name %s is speed %f"), *GetName(), Speed);
	}
}

void ABall::SetBallState(const EState NewState)
{
	State = NewState;
}