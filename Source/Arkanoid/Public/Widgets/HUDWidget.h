// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Framework/ArkanoidGameState.h"
#include "Framework/ArkanoidPlayerState.h"
#include "HUDWidget.generated.h"


UCLASS()
class ARKANOID_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* CurrentScore = nullptr;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* GameTime = nullptr;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* LevelRecord = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* ShakeAnimation = nullptr;

	UPROPERTY()
	AArkanoidPlayerState* PlayerState = nullptr;

	UPROPERTY()
	AArkanoidGameState* GameState = nullptr;

protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
	void UpdateScore(const int32 NewScore);
	void UpdateGameTime();
	void UpdateRecordOnScreen();
};
