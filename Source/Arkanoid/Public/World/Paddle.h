// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
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
public:
	
	APaddle();

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

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
};