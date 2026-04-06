<<<<<<< HEAD
=======
// Alexandr Sharizanov. All rights reserved.


>>>>>>> c81d547 (Добавьте файлы проекта.)
#include "World/Spawner.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Arrow"));
	ForwardArrow->SetupAttachment(SceneRoot);
	ForwardArrow->SetAbsolute(false, false, true);
}

void ASpawner::SpawnActor()
{
	if (SpawnedClass)
	{
		if (const auto World = GetWorld())
		{
			World->SpawnActor<AActor>(SpawnedClass, ForwardArrow->GetComponentLocation(), ForwardArrow->GetComponentRotation());
		}
	}
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	// Сразу спавним один актор
	SpawnActor();

	// Запускаем повторяющийся таймер — каждые SpawnInterval секунд
	if (GetWorld() && SpawnInterval > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::SpawnActor, SpawnInterval, true, SpawnInterval);
	}
}