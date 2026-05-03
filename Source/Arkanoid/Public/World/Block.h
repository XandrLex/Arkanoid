// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/LifeComponent.h"
#include "Bonuses/BonusParent.h"
#include "Block.generated.h"

UCLASS()
class ARKANOID_API ABlock : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	ULifeComponent* LifeComponent = nullptr;

	TSubclassOf<ABonusParent> BonusClass = nullptr;

	int32 MaxLife = 1;

public:	
	ABlock();

	void Init(const FVector NewScale, const int32 LifeAmount, const TSubclassOf<ABonusParent> NewBonusClass = nullptr);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ScoreByLife = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<UMaterialInterface*> LifeMaterials;

protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
		FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void BeginPlay() override;
};
