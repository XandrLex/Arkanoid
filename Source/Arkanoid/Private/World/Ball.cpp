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

	// Защита от повторной обработки одного и того же пересечения
	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	if (Now - LastCollisionTime < CollisionIgnoreInterval)
		return;
	// пометим время сразу — чтобы в случае ранних return не обработать то же столкновение снова
	LastCollisionTime = Now;

	AActor* OtherActor = HitResult.GetActor();
	UPrimitiveComponent* OtherComp = HitResult.GetComponent();
	// Надёжная нормаль столкновения: предпочитаем ImpactNormal, fallback на Normal
	FVector ImpactNormal = HitResult.ImpactNormal.IsNearlyZero() ? HitResult.Normal : HitResult.ImpactNormal;
	ImpactNormal.Z = 0.0f;
	ImpactNormal = ImpactNormal.GetSafeNormal();

	// УДАР О ПЛАТФОРМУ
	if (auto Paddle = Cast<APaddle>(OtherActor))
	{
		// Sticky логика
		if (Paddle->TryAttachBall(this))
		{
			return;
		}

		// Оригинальная логика: угол по относительному смещению от центра платформы
		const FVector PaddleLocation = Paddle->GetActorLocation();
		const FVector BallLocation = GetActorLocation();

		float RelativeHit = (BallLocation.Y - PaddleLocation.Y) / Paddle->GetWidth();
		RelativeHit = FMath::Clamp(RelativeHit, -1.0f, 1.0f);

		FVector NewDirection = FVector(1.0f, RelativeHit, 0.0f);
		NewDirection.Y += Paddle->GetVelocity().Y * 0.002f;

		Direction = NewDirection.GetSafeNormal();

		if (Speed < InitParameters.MaxSpeed)
		{
			Speed += InitParameters.Speed * 0.05f;
			Speed = FMath::Min(Speed, InitParameters.MaxSpeed);
		}
		return;
	}

	// УДАР О ДРУГОЙ ШАРИК
	if (auto OtherBall = Cast<ABall>(OtherActor))
	{
		// Если OtherBall прикреплён к каретке — отразим по нормали
		const bool bOtherAttachedToPaddle =
			(OtherBall->GetAttachParentActor() && Cast<APaddle>(OtherBall->GetAttachParentActor()) != nullptr)
			|| (OtherBall->GetOwner() && Cast<APaddle>(OtherBall->GetOwner()) != nullptr);

		// Позиции и нормаль (от Other к этому)
		const FVector Pos = GetActorLocation();
		const FVector OtherPos = OtherBall->GetActorLocation();
		FVector N = (Pos - OtherPos);
		if (N.IsNearlyZero())
		{
			N = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f);
		}
		N.Z = 0.0f;
		N = N.GetSafeNormal();

		if (bOtherAttachedToPaddle)
		{
			const float Dot = FVector::DotProduct(Direction, N);
			FVector Reflected = Direction - 2.0f * Dot * N;
			Reflected.Z = 0.0f;

			const float MinForward = 0.25f;
			if (FMath::Abs(Reflected.X) < MinForward)
			{
				Reflected.X = FMath::Sign(Reflected.X == 0.0f ? 1.0f : Reflected.X) * MinForward;
			}

			Direction = Reflected.GetSafeNormal();

			const float Separation = 8.0f;
			AddActorWorldOffset(N * Separation, true);

			if (Speed < InitParameters.MaxSpeed)
			{
				Speed += InitParameters.Speed * 0.02f;
				Speed = FMath::Min(Speed, InitParameters.MaxSpeed);
			}
			return;
		}

		// Полное упругое столкновение (обмен нормальных компонент для равных масс)
		const FVector V1 = Direction * Speed;
		const FVector V2 = OtherBall->Direction * OtherBall->Speed;
		const FVector T = FVector(-N.Y, N.X, 0.0f);

		const float V1n = FVector::DotProduct(V1, N);
		const float V1t = FVector::DotProduct(V1, T);
		const float V2n = FVector::DotProduct(V2, N);
		const float V2t = FVector::DotProduct(V2, T);

		const float V1nAfter = V2n;
		const float V2nAfter = V1n;

		const FVector NewV1 = T * V1t + N * V1nAfter;
		const FVector NewV2 = T * V2t + N * V2nAfter;

		FVector FinalV1 = NewV1;
		FVector FinalV2 = NewV2;
		if (FinalV1.IsNearlyZero())
			FinalV1 = N * Speed;
		if (FinalV2.IsNearlyZero())
			FinalV2 = -N * OtherBall->Speed;

		Direction = FinalV1.GetSafeNormal();
		Speed = FinalV1.Size();
		OtherBall->Direction = FinalV2.GetSafeNormal();
		OtherBall->Speed = FinalV2.Size();

		OtherBall->SetBallState(EState::Moving);
		SetBallState(EState::Moving);

		const float Separation = 8.0f;
		AddActorWorldOffset(N * Separation, true);
		OtherBall->AddActorWorldOffset(-N * Separation, true);

		return;
	}

	// УДАР О БЛОК ИЛИ СТЕНЫ — отражение по нормали удара (ImpactNormal предпочтительнее)
	if (!ImpactNormal.IsNearlyZero())
	{
		const float Dot = FVector::DotProduct(Direction, ImpactNormal);
		FVector Reflected = Direction - 2.0f * Dot * ImpactNormal;
		Reflected.Z = 0.0f;

		// Учитываем движение поверхности (мягко) — это помогает при движущихся/вращающихся мешах
		if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			Reflected += OtherComp->GetComponentVelocity() * 0.001f;
		}
		else if (OtherActor)
		{
			Reflected += OtherActor->GetVelocity() * 0.001f;
		}

		const float MinForward = 0.25f;
		if (FMath::Abs(Reflected.X) < MinForward)
		{
			Reflected.X = FMath::Sign(Reflected.X == 0.0f ? 1.0f : Reflected.X) * MinForward;
		}

		Reflected.Y += FMath::RandRange(-0.05f, 0.05f);

		Direction = Reflected.GetSafeNormal();

		const float Separation = 6.0f;
		AddActorWorldOffset(ImpactNormal * Separation, false);

		if (Speed < InitParameters.MaxSpeed)
		{
			Speed += InitParameters.Speed * 0.1f;
			Speed = FMath::Min(Speed, InitParameters.MaxSpeed);
		}
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

void ABall::SetBallBonus(const float BallLifeTime)
{
	if (StaticMesh)
	{
		StaticMesh->SetVectorParameterValueOnMaterials(
			"Emissive color 01", FVector(40.0f, 40.0f, 0.03f));
		StaticMesh->SetVectorParameterValueOnMaterials(
			"Emissive color 02", FVector(40.0f, 40.0f, 0.03f));
	}

	SetLifeSpan(BallLifeTime);
	Launch(GetActorForwardVector(), Speed);
}
