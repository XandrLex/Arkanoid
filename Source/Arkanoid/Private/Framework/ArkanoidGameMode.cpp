// Alexandr Sharizanov. All rights reserved.


#include "Framework/ArkanoidGameMode.h"
#include "Framework/ArkanoidGameState.h"
#include "Framework/ArkanoidPlayerState.h"	


AArkanoidGameMode::AArkanoidGameMode()
{
	GameStateClass = AArkanoidGameState::StaticClass();
	PlayerStateClass = AArkanoidPlayerState::StaticClass();
}

void AArkanoidGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameStarted();
}

void AArkanoidGameMode::GameStarted()
{
	auto ArkanoidGameState = Cast<AArkanoidGameState>(GameState);
	if (ArkanoidGameState)
	{
		ArkanoidGameState->StartGame();
	}
}

void AArkanoidGameMode::GameEnded()
{
	auto ArkanoidGameState = Cast<AArkanoidGameState>(GameState);
	if (ArkanoidGameState)
	{
		ArkanoidGameState->StopGame();
	}

	UE_LOG(LogTemp, Warning, TEXT("Game Ended!"));
}
