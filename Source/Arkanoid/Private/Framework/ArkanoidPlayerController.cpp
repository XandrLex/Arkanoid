// Alexandr Sharizanov. All rights reserved.


#include "Framework/ArkanoidPlayerController.h"
#include "Framework/ArkanoidHUD.h"


void AArkanoidPlayerController::ExitButtonPressed()
{
	const auto CurrentHUD = Cast<AArkanoidHUD>(GetHUD());
	if (!CurrentHUD)
		return;

	if (IsPaused())
	{
		SetPause(false);
		CurrentHUD->HidePauseWidget();
	}
	else
	{
		SetPause(true);
		CurrentHUD->ShowPauseWidget();
	}
}

void AArkanoidPlayerController::ShowGameEndMenu(const bool bWinStatus)
{
	const auto CurrentHUD = Cast<AArkanoidHUD>(GetHUD());
	if (CurrentHUD)
	    CurrentHUD->ShowGameEndWidget(bWinStatus);

	SetPause(true);
}
