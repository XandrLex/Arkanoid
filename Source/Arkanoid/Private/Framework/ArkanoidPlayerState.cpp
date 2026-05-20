// Alexandr Sharizanov. All rights reserved.


#include "Framework/ArkanoidPlayerState.h"
#include "Framework/ArkanoidGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AArkanoidPlayerState::ChangePlayerScore(const int32 Amount)
{
	PlayerScore = FMath::Max(0, PlayerScore + Amount);

	if (const auto GI = Cast<UArkanoidGameInstance>(GetGameInstance()))
		GI->SetLevelRecord(UGameplayStatics::GetCurrentLevelName(this), PlayerScore);

	OnPlayerScoreChanged.Broadcast(PlayerScore);
	UE_LOG(LogTemp, Log, TEXT("Player score = %d"), PlayerScore);
}

