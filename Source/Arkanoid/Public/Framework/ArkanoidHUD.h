// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/HUDWidget.h"
#include "Widgets/PauseWidget.h"
#include "ArkanoidHUD.generated.h"

UCLASS()
class ARKANOID_API AArkanoidHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UHUDWidget* HUDWidget = nullptr;
	UPROPERTY()
	UPauseWidget* PauseWidget = nullptr;
	
	void ChangeInputMode(UWidget* Widget = nullptr) const;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<UHUDWidget> HUDWidgetClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<UPauseWidget> PauseWidgetClass = nullptr;

	void ShowPauseWidget();
	UFUNCTION()
	void HidePauseWidget();
	void ShowGameEndWidget(const bool bWinStatus);
	
};
