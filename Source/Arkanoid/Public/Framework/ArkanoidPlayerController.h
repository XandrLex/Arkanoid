// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArkanoidPlayerController.generated.h"


UCLASS()
class ARKANOID_API AArkanoidPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ExitButtonPressed();
	void ShowGameEndMenu(const bool bWinStatus);
	
};
