// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArkanoidGameMode.generated.h"


UCLASS()
class ARKANOID_API AArkanoidGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	AArkanoidGameMode();

	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void GameStarted();
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void GameEnded();
};
