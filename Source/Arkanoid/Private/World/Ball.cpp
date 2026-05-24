// Alexandr Sharizanov. All rights reserved.


#include "World/Ball.h"
#include "Components/ArrowComponent.h"
#include "World/Paddle.h"
#include "World/Block.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

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

	if (!HitResult.bBlockingHit)
		return;

	AActor* OtherActor = HitResult.GetActor();

	// УДАР О ПЛАТФОРМУ
	if (auto Paddle = Cast<APaddle>(OtherActor))
	{
		// Sticky логика
		if (Paddle->TryAttachBall(this))
		{
			return;
		}

		// Вычисляем смещение относительно центра платформы
		const FVector PaddleLocation = Paddle->GetActorLocation();
		const FVector BallLocation = GetActorLocation();

		float RelativeHit = (BallLocation.Y - PaddleLocation.Y) / Paddle->GetWidth();
		RelativeHit = FMath::Clamp(RelativeHit, -1.0f, 1.0f);

		// Формируем новое направление (вперёд + угол)
		FVector NewDirection = FVector(1.0f, RelativeHit, 0.0f);

		// Добавляем влияние движения платформы
		NewDirection.Y += Paddle->GetVelocity().Y * 0.002f;

		Direction = NewDirection.GetSafeNormal();

		// Чуть ускоряем при ударе
		if (Speed < InitParameters.MaxSpeed)
		{
			Speed += InitParameters.Speed * 0.05f;
			Speed = FMath::Min(Speed, InitParameters.MaxSpeed);
		}
		return;
	}

	// УДАР О БЛОК ИЛИ СТЕНЫ
	// Если шар в режиме огненного шарика и попал в блок, НЕ отражаем его — позволяем блоку обработать попадание и уничтожиться,
	// а шар продолжит движение (отталкивается только от стен и каретки).
	if (bIsFireBall)
	{
		if (auto Block = Cast<ABlock>(OtherActor))
		{
			// Не меняем Direction — просто выходим, чтобы блок провёл свою логику (NotifyHit в ABlock)
			return;
		}
	}

	// Обычное отражение по нормали
	const FVector Normal = HitResult.Normal;

	// Упрощённое отражение (аркадное)
	if (FMath::Abs(Normal.X) > 0.9f)
	{
		Direction.X *= -1.0f;
	}
	else
	{
		Direction.Y *= -1.0f;
	}

	// АНТИ-ЗАЦИКЛИВАНИЕ
	// Маленький рандом
	Direction.Y += FMath::RandRange(-0.1f, 0.1f);

	// Минимальный вертикальный угол
	const float MinForward = 0.25f;

	if (FMath::Abs(Direction.X) < MinForward)
	{
		Direction.X = FMath::Sign(Direction.X) * MinForward;
	}

	Direction = Direction.GetSafeNormal();

	// УСКОРЕНИЕ
	if (Speed < InitParameters.MaxSpeed)
	{
		Speed += InitParameters.Speed * 0.1f;
		Speed = FMath::Min(Speed, InitParameters.MaxSpeed);
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

// Включить режим огненного шарика на Duration секунд.
// Если Duration <= 0.0f — режим включается без таймера (можно выключить вручную через DisableFireBall).
void ABall::EnableFireBall(float Duration)
{
	bIsFireBall = true;

	if (Duration > 0.0f && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerFireBall);
		GetWorld()->GetTimerManager().SetTimer(TimerFireBall, this, &ABall::DisableFireBall, Duration, false);
	}
}

// Выключить режим огненного шарика
void ABall::DisableFireBall()
{
	bIsFireBall = false;
}
