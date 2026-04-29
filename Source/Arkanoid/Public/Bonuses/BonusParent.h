// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <World/Paddle.h>
#include "BonusParent.generated.h"


UCLASS(Blueprintable, Abstract)
class ARKANOID_API ABonusParent : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BonusMesh = nullptr;

public:	
	
	ABonusParent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (Tooltip = "Направление движения"))
	FVector Direction = FVector(-1.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (Tooltip = "Продолжительность действия"))
	float Duration = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (Tooltip = "Скорость движения"))
	float Speed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (Tooltip = "Размер"))
	float Value = 0.5f;

	void InitScale(const FVector NewScale);

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void MoveAction(const float DeltaTime);
	virtual void BonusAction(APaddle* Paddle);
};
