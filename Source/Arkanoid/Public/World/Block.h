// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

class ULifeComponent;

UCLASS()
class ARKANOID_API ABlock : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	ULifeComponent* LifeComponent = nullptr;
	TSubclassOf<AActor> BonusClass = nullptr;

public:	
	ABlock();
	void Init(const FVector NewScale, const int32 LifeAmount, const TSubclassOf<AActor> NewBonusClass = nullptr);
	
protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
		FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

};