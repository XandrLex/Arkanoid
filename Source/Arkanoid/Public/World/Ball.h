// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadEvent);

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle,
	Moving
};

USTRUCT(BlueprintType)
struct FInitParameters
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Tooltip = "Стартовый размер"))
	float Scale;
	UPROPERTY(EditDefaultsOnly, meta = (Tooltip = "Начальная сила"))
	int32 Power;
	UPROPERTY(EditDefaultsOnly, meta = (Tooltip = "Начальная скорость"))
	float Speed;
	UPROPERTY(EditDefaultsOnly, meta = (Tooltip = "Максимальная скорость"))
	float MaxSpeed;

	FInitParameters()
	{
		Scale = 0.5f;
		Power = 1;
		Speed = 500.0f;
		MaxSpeed = 2500.0f;
	}
};

UCLASS()
class ARKANOID_API ABall : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ForwardArrow = nullptr;

	int32 Power = 1;
	float Speed = 500.0f;
	FVector Direction = FVector::ZeroVector;
	EState State = EState::Idle;

public:	
    ABall();
	FORCEINLINE int32 GetPower() const { return Power; }
	UPROPERTY(BlueprintAssignable)
	FOnDeadEvent OnDeadEvent;
	/**
	* Функция для установки состояния шарика.
	* @param NewState - новое состояние шарика.
	*/
	void SetBallState(const EState NewState);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	FInitParameters InitParameters;
	/**
	* Функция для обработки движения шарика.
	* @param DeltaTime - время, прошедшее с последнего кадра.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ball")
	void Move(const float DeltaTime);
};