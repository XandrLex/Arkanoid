// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class ARKANOID_API ASpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ForwardArrow = nullptr;

	// Таймер спауна
	FTimerHandle SpawnTimerHandle;

public:	
	ASpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AActor> SpawnedClass = nullptr;

	// Интервал спауна в секундах
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SpawnInterval = 2.0f;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnActor();

protected:
	virtual void BeginPlay() override;
};