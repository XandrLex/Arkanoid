// Alexandr Sharizanov. All rights reserved.


#include "World/MyKillZone.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

AMyKillZone::AMyKillZone()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Arrow"));
	ForwardArrow->SetupAttachment(SceneRoot);
	ForwardArrow->SetAbsolute(false, false, true);
	KillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Kill Box"));
	KillBox->SetupAttachment(SceneRoot);
	KillBox->SetBoxExtent(FVector(50.0f, 500.0f, 50.0f));
}

void AMyKillZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(OtherActor && OtherActor != this)
	{
		OtherActor->Destroy();
	}
}