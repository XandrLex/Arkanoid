// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "InputMappingContext.h"
#include "Paddle.generated.h"

UCLASS()
class ARKANOID_API APaddle : public APawn
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UBoxComponent* BoxCollider = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UStaticMeshComponent* StaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UStaticMeshComponent* LeftStaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UStaticMeshComponent* RightStaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UArrowComponent* Arrow = nullptr;
	UPROPERTY()
	ABall* CurrentBall = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Settings | Input")
	UInputMappingContext* DefaultMappingContext = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Settings | Input")
	UInputAction* EscapeAction = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Settings | Input")
	UInputAction* SpawnBallAction = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Settings | Input")
	UInputAction* MoveAction = nullptr;
	UPROPERTY()
	TArray<UStaticMeshComponent*> BallLives;
	void SpawnBallLives();
	void UpdateBallLivesLocation();

public:
	
	APaddle();

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void ExitGame();
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void SpawnBall();
	UFUNCTION()
	void BallIsDead();

public:	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings | Game")
	TSubclassOf<ABall> BallClass = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Settings | Game")
	int32 Lives = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Settings | Game")
	float Speed = 2000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Settings | Game")
	FVector DefaultScale = FVector(0.4f, 2.4f, 0.5f);

	//Работа с бонусами
public:

	void BonusChangeSize(const float AdditionalSize, const float BonusTime);
	void BonusChangeLife(const int32 Amount);
	void BonusChangeBallSpeed(const float AdditionalSpeed);
	void BonusChangeBallPower(const int32 Amount, const float BonusTime);
	// Включить бонус "прилипший шарик" на каретке на время BonusTime
	void BonusStickyBall(const float BonusTime);
	// Попытка прикрепить указанный шарик к этой каретке. Возвращает true, если прикреплено.
	bool TryAttachBall(ABall* Ball);

protected:

	FTimerHandle TimerForBonusSize;
	FTimerHandle TimerForBonusSticky;

	UFUNCTION()
	void SetDefaultSize();
	// Флаг активированного режима прилипшего шарика
	bool bStickyEnabled = false;
	// Отключить режим прилипшего шарика
	void DisableSticky();
};
