// Alexandr Sharizanov. All rights reserved.


#include "World/Paddle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/ArkanoidGameMode.h"
#include "Framework/ArkanoidPlayerController.h"
#include "Kismet/GameplayStatics.h"




void APaddle::SpawnBallLives()
{
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial_Inst.BasicShapeMaterial_Inst"));
	if (!Mesh || !Material)
	{
		return;
	}

	for (auto BallLive : BallLives)
	{
		BallLive->DestroyComponent();
	}
	BallLives.Empty();

	for (int8 i = 0; i < Lives - 1; ++i)
	{
		auto NewMeshComponent = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("Lives %d"), i + 1));
		if (NewMeshComponent)
		{
			NewMeshComponent->SetStaticMesh(Mesh);
			NewMeshComponent->SetMaterial(0, Material);
			NewMeshComponent->SetAbsolute(false, false, true);
			NewMeshComponent->SetWorldScale3D(FVector(0.5f));
			NewMeshComponent->SetupAttachment(StaticMesh);
			NewMeshComponent->RegisterComponent();
			BallLives.Add(NewMeshComponent);
		}
	}
	UpdateBallLivesLocation();
}

void APaddle::UpdateBallLivesLocation()
{
	constexpr float BallSpacing = 30.0f;
	const int8 NumBalls = BallLives.Num();
	const float TotalWidth = (NumBalls - 1) * BallSpacing;
	const float StartOffset = -TotalWidth / 2.0f;
	for (int8 i = 0; i < NumBalls; ++i)
	{
		const float Offset = StartOffset + i * BallSpacing;
		if (IsValid(BallLives[i]))
		{
			BallLives[i]->SetRelativeLocation(FVector(-100.0f, Offset, 0.0f));
		}
	}
}

APaddle::APaddle()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetBoxExtent(FVector(25.0f, 50.0f, 25.0f));
	BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(BoxCollider);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(BoxCollider);

	LeftStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Static Mesh"));
	LeftStaticMesh->SetupAttachment(StaticMesh);
	LeftStaticMesh->AddRelativeLocation(FVector(0.0f, -50.0f, 0.0f));
	LeftStaticMesh->SetAbsolute(false, false, true);

	RightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Static Mesh"));
	RightStaticMesh->SetupAttachment(StaticMesh);
	RightStaticMesh->AddRelativeLocation(FVector(0.0f, 50.0f, 0.0f));	
	RightStaticMesh->SetAbsolute(false, false, true);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(StaticMesh);
	Arrow->AddRelativeLocation(FVector(150.0f, 0.0f, 0.0f));
	Arrow->SetAbsolute(false, false, true);
}


void APaddle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(DefaultScale);
	BoxCollider->SetBoxExtent(FVector(25.0f, 50.0f + 20.0f / DefaultScale.Y, 25.0f));
	const FVector TempScale = FVector(GetActorScale().X, GetActorScale().X, GetActorScale().Z);
	LeftStaticMesh->SetWorldScale3D(TempScale);
	RightStaticMesh->SetWorldScale3D(TempScale);
}

void APaddle::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SpawnBall();
	SpawnBallLives();
}

void APaddle::ExitGame()
{
	if (const auto PC = Cast<AArkanoidPlayerController>(Controller))
	{
		PC->ExitButtonPressed();
	}
}

void APaddle::StartGame()
{
	if (CurrentBall && CurrentBall->GetAttachParentActor() == this)
	{
		CurrentBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		FVector Dir = Arrow->GetForwardVector();
		CurrentBall->Launch(Dir, 500.0f);
	}
}

void APaddle::Move(const FInputActionValue& Value)
{
	const FVector2D AxisVector = Value.Get<FVector2D>();
	if(Controller)
	{
		const float CurrentSpeed = AxisVector.X * Speed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddActorWorldOffset(FVector(0.0f, CurrentSpeed, 0.0f), true);
	}
}

void APaddle::SpawnBall()
{
	if(BallClass && !CurrentBall)
	{
		FTransform SpawnTransform = Arrow->GetComponentTransform();
		CurrentBall = GetWorld()->SpawnActor<ABall>(BallClass, SpawnTransform);
		if (CurrentBall)
		{
			CurrentBall->SetOwner(this);
			CurrentBall->SetBallState(EState::Idle);
			CurrentBall->OnDeadEvent.AddDynamic(this, &APaddle::BallIsDead);
			CurrentBall->AttachToComponent(Arrow, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void APaddle::BallIsDead()
{
	CurrentBall = nullptr;
	Lives = FMath::Max(Lives - 1, 0);
	if(Lives)
	{
		SpawnBall();
		BallLives[Lives - 1]->DestroyComponent();
		BallLives.RemoveAt(Lives - 1);
		UpdateBallLivesLocation();
	}
	else
	{
		if (const auto Gm = Cast<AArkanoidGameMode>(GetWorld()->GetAuthGameMode()))
		{
			Gm->GameEnded(false);
		}
	}
}

void APaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Started, this, &APaddle::ExitGame);
		EnhancedInputComponent->BindAction(SpawnBallAction, ETriggerEvent::Started, this, &APaddle::StartGame);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaddle::Move);
	}
}

void APaddle::BonusChangeSize(const float AdditionalSize, const float BonusTime)
{
	if(AdditionalSize && BonusTime)
	{
		if(!GetWorld()->GetTimerManager().IsTimerActive(TimerForBonusSize))
		{
			FVector TempScale = GetActorScale3D();
			TempScale.Y = TempScale.Y + TempScale.Y * AdditionalSize;
			SetActorScale3D(TempScale);
			BoxCollider->SetBoxExtent(FVector(25.0f, 50.0f + 20.0f / TempScale.Y, 25.0f));
		}
		GetWorld()->GetTimerManager().SetTimer(TimerForBonusSize, this, &APaddle::SetDefaultSize, BonusTime, false);
		
			
	}
}

void APaddle::BonusChangeLife(const int32 Amount)
{
	Lives += Amount;
	SpawnBallLives();
}

void APaddle::BonusChangeBallSpeed(const float AdditionalSpeed)
{
	if (IsValid(CurrentBall))
	{
		CurrentBall->ChangeSpeed(AdditionalSpeed);
	}
}

void APaddle::BonusChangeBallPower(const int32 Amount, const float BonusTime)
{
	if (IsValid(CurrentBall))
	{
		CurrentBall->ChangeBallPower(Amount, BonusTime);
	}
}

void APaddle::SetDefaultSize()
{
	SetActorScale3D(DefaultScale);
	BoxCollider->SetBoxExtent(FVector(25.0f, 50.0f + 20.0f / DefaultScale.Y, 25.0f));
}

// Включает режим "прилипший шарик" на каретке на время BonusTime
void APaddle::BonusStickyBall(const float BonusTime)
{
	if (BonusTime <= 0.0f)
	{
		// Если время не задано — просто включаем флаг без таймера
		bStickyEnabled = true;
		return;
	}

	// Включаем режим, сбрасываем таймер и запускаем новый
	if (!bStickyEnabled)
	{
		bStickyEnabled = true;
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerForBonusSticky);
	GetWorld()->GetTimerManager().SetTimer(TimerForBonusSticky, this, &APaddle::DisableSticky, BonusTime, false);
}

// Отключает режим "прилипший шарик"
void APaddle::DisableSticky()
{
	bStickyEnabled = false;
}

// Попытка прикрепить шарик при его столкновении с кареткой
bool APaddle::TryAttachBall(ABall* Ball)
{
	// Не допускаем прикрепление, если шар некорректен или режим липкости отключён
	if (!IsValid(Ball))
	{
		return false;
	}
	if (!bStickyEnabled)
	{
		return false;
	}

	// Проверка: есть ли уже шар, прикреплённый к этой каретке.
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		if (Actor && Cast<ABall>(Actor))
		{
			// Если уже есть прикреплённый шар (любой), отклоняем новую попытку.
			return false;
		}
	}

	// Дополнительная защита: CurrentBall может содержать ссылку на ранее созданный шар, убедимся что он не прикреплён сейчас
	if (IsValid(CurrentBall) && CurrentBall->GetAttachParentActor() == this)
	{
		return false;
	}

	CurrentBall = Ball;
	Ball->SetOwner(this);
	Ball->SetBallState(EState::Idle);
	Ball->AttachToComponent(Arrow, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	return true;
}

float APaddle::GetWidth() const
{
	return BoxCollider->GetScaledBoxExtent().Y;
}

void APaddle::BonusBall(const float BallLifeTime) const
{
	if (BallClass)
	{
		// Сдвигаем точку спавна вперёд по направлению Arrow, чтобы шар не пересекался с кареткой сразу
		FVector SpawnLocation = Arrow->GetComponentLocation() + Arrow->GetForwardVector() * 150.0f;
		const FRotator SpawnRotation = Arrow->GetComponentRotation();

		if (IsValid(CurrentBall))
			SpawnLocation.X = SpawnLocation.X + 50.0f;

		const auto BonusBall = GetWorld()->SpawnActor<ABall>(BallClass, SpawnLocation, SpawnRotation);
		if (BonusBall)
			BonusBall->SetBallBonus(BallLifeTime);
	}
}
