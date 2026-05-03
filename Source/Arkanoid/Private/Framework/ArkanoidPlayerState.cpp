// Alexandr Sharizanov. All rights reserved.


#include "Framework/ArkanoidPlayerState.h"

void AArkanoidPlayerState::ChangePlayerScore(const int32 Amount)
{
	PlayerScore = FMath::Max(0, PlayerScore + Amount);
	OnPlayerScoreChanged.Broadcast(PlayerScore);
	UE_LOG(LogTemp, Log, TEXT("Player score = %d"), PlayerScore);
}

