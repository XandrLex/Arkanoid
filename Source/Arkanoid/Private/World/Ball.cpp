// Alexandr Sharizanov. All rights reserved.


#include "World/Ball.h"
#include "Components/ArrowComponent.h"
#include <World/Paddle.h>

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

	if (StaticMesh)
	{
		DefaultMaterial = StaticMesh->GetMaterial(0);
	}
	UpdateBallMaterial();
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

void ABall::Destroyed()
{
	OnDeadEvent.Broadcast();
	Super::Destroyed();
}

void ABall::Move(const float DeltaTime)
{
	const FVector Offset = Direction * Speed * DeltaTime;
	FHitResult HitResult;
	AddActorWorldOffset(Offset, true, &HitResult);
	if(HitResult.bBlockingHit)
	{
		// ѕопытка обработать столкновение с кареткой: если у каретки активен sticky Ч прикрепл€ем шарик
		if (AActor* OtherActor = HitResult.GetActor())
		{
			if (auto Paddle = Cast<APaddle>(OtherActor))
			{
				// TryAttachBall выполнит проверку на существующий CurrentBall и флаг sticky
				if (Paddle->TryAttachBall(this))
				{
					// ѕрисоединение прошло Ч прекращаем дальнейшую обработку отражени€
					return;
				}
			}
		}

		// —тандартное отражение от поверхности
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

void ABall::ChangeSpeed(const float AdditionalSpeed)
{
	if (AdditionalSpeed < 0.0f)
	{
		Speed = FMath::Min(Speed - Speed * AdditionalSpeed, InitParameters.Speed);
	}
	else if (AdditionalSpeed > 0.0f)
	{
		Speed = FMath::Max(Speed + Speed * AdditionalSpeed, InitParameters.MaxSpeed);
	}
}

void ABall::ChangeBallPower(const int32 Amount, const float BonusTime)
{
	if (Amount != 0 && BonusTime > 0.0f)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerBallPower))
		{
			Power = FMath::Max(Power + Amount, 1);
			UpdateBallMaterial();
		}
		GetWorld()->GetTimerManager().SetTimer(TimerBallPower, this, &ABall::ResetBallPower, BonusTime, false);
	}
}

void ABall::Launch(const FVector& LaunchDirection, float LaunchSpeed)
{
	Direction = LaunchDirection.GetSafeNormal();
	Speed = LaunchSpeed;
	SetBallState(EState::Moving);
}

void ABall::ResetBallPower()
{
	Power = InitParameters.Power;
	UpdateBallMaterial();
}

void ABall::UpdateBallMaterial()
{
	if (!StaticMesh)
		return;
	if (Power > 1)
	{
		if (PowerMaterial)
		{
			StaticMesh->SetMaterial(0, PowerMaterial);
		}
	}
	else
	{
		StaticMesh->SetMaterial(0, DefaultMaterial);
	}
}
