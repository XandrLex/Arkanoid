// Alexandr Sharizanov. All rights reserved.


#include "Widgets/PauseWidget.h"
#include "Kismet/GameplayStatics.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ResumeButton)
	{
		ResumeButton->OnReleased.AddDynamic(this, &UPauseWidget::ResumeGame);
	}
	if (RestartButton)
	{
		RestartButton->OnReleased.AddDynamic(this, &UPauseWidget::RestartGame);
	}
	if (MenuButton)
	{
		MenuButton->OnReleased.AddDynamic(this, &UPauseWidget::BackToMenu);
	}
}

void UPauseWidget::ResumeGame()
{
	OnGameResume.Broadcast();
}

void UPauseWidget::RestartGame()
{
	const auto LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

void UPauseWidget::BackToMenu()
{
	UGameplayStatics::OpenLevel(this, FName("Menu"));
}

void UPauseWidget::SetWinStatus(const bool bWinStatus)
{
	if (!StatusText)
		return;

	UGameplayStatics::PlaySound2D(this, bWinStatus ? WinSound : LoseSound);

	if (ResumeButton)
	{
		ResumeButton->SetIsEnabled(false);
		ResumeButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (bWinStatus)
	{
		StatusText->SetText(FText::FromString("YOU WIN!!!"));
		StatusText->SetColorAndOpacity(FColor::Green);
	}
	else
	{
		StatusText->SetText(FText::FromString("YOU LOSE!"));
		StatusText->SetColorAndOpacity(FColor::Red);
	}
}
