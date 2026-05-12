// Alexandr Sharizanov. All rights reserved.


#include "Widgets/HUDWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayer())
	{
		PlayerState = GetOwningPlayer()->GetPlayerState<AArkanoidPlayerState>();
		if (PlayerState)
		{
			PlayerState->OnPlayerScoreChanged.AddDynamic(this, &UHUDWidget::UpdateScore);
		}
	}
	if (GetWorld())
	{
		GameState = Cast<AArkanoidGameState>(GetWorld()->GetGameState());
	}
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateGameTime();
}

void UHUDWidget::UpdateScore(const int32 NewScore)
{
	if (CurrentScore)
	{
		const FString ScoreString = FString::Printf(TEXT("Score: %03d"), NewScore);
		CurrentScore->SetText(FText::FromString(ScoreString));

		if (ShakeAnimation)
		{
			PlayAnimation(ShakeAnimation, 0.0f, 3, EUMGSequencePlayMode::Forward, 1.0f);
		}
	}
}

void UHUDWidget::UpdateGameTime()
{
	if (GameState && GameTime)
	{
		int32 Minutes = 0;
		int32 Seconds = 0;
		int32 Milliseconds = 0;
		GameState->GetGameTime(Minutes, Seconds, Milliseconds);
		const FString TimeString = FString::Printf(TEXT("%02d : %02d : %02d"), Minutes, Seconds, Milliseconds);
		GameTime->SetText(FText::FromString(TimeString));
	}
}
