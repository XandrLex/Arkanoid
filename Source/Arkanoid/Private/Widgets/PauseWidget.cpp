// Alexandr Sharizanov. All rights reserved.


#include "Widgets/PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/ArkanoidGameInstance.h"

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
	if (NextLevelButton)
	{
		NextLevelButton->OnReleased.AddDynamic(this, &UPauseWidget::NextLevel);
		// По умолчанию скрываем кнопку — будет отображаться только при победе
		NextLevelButton->SetIsEnabled(false);
		NextLevelButton->SetVisibility(ESlateVisibility::Hidden);
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

void UPauseWidget::NextLevel()
{
	// Получаем экземпляр GameInstance и просим открыть следующий уровень
	if (const auto GI = Cast<UArkanoidGameInstance>(GetGameInstance()))
	{
		const FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(this);
		GI->OpenNextLevel(CurrentLevel);
	}
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

	// Управление видимостью и доступностью NextLevelButton в зависимости от статуса
	if (NextLevelButton)
	{
		if (bWinStatus)
		{
			NextLevelButton->SetIsEnabled(true);
			NextLevelButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			NextLevelButton->SetIsEnabled(false);
			NextLevelButton->SetVisibility(ESlateVisibility::Hidden);
		}
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
