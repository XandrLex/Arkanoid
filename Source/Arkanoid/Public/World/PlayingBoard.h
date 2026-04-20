// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.h"
#include "PlayingBoard.generated.h"


USTRUCT(BlueprintType)
struct FBonusTypeChance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BonusClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DropChance = 0.2f;
};

UCLASS()
class ARKANOID_API APlayingBoard : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY()
	TArray<UStaticMeshComponent*> MyPreviewComponents;

	UPROPERTY()
	TArray<ABlock*> BlockActors;
	
	void CreateMyPreviewComponents();
	void ClearMyPreviewComponents();
	void SpawnBlockActors();
	UFUNCTION()
	void OnBlockDestroyed(AActor* DestroyedBlock);

public:	

	APlayingBoard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Base", meta = (Tooltip = "Блюпринт блока"))
	TSubclassOf<ABlock> BlockClassForSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Base", meta = (Tooltip = "Меш для превью"))
	UStaticMesh* PreviewMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Base", meta = (Tooltip = "Размер блока"))
	FVector BlockScale = FVector(0.5f, 0.5f, 0.5f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Base", meta = (Tooltip = "Размер сетки по X"))
	int32 GridSizeX = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Base", meta = (Tooltip = "Размер сетки по Y"))
	int32 GridSizeY = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Base", meta = (Tooltip = "Расстояние между блоками по X"))
	int32 SpacingX = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Base", meta = (Tooltip = "Расстояние между блоками по Y"))
	int32 SpacingY = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Game", meta = (ClampMin = "0.0", ClampMax = "1.0"), meta = (Tooltip = "Сложность игры"))
	float GameDifficulty = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Game", meta = (ClampMin = "0.0", ClampMax = "1.0"), meta = (Tooltip = "Шанс выпадения бонуса"))
	float BonusChance = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Game")
	TArray<FBonusTypeChance> BonusTypeByChance;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	TSubclassOf<AActor> GetBonusClass();
};