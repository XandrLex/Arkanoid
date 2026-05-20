// Alexandr Sharizanov. All rights reserved.


#include "Framework/ArkanoidGameMode.h"
#include "Framework/ArkanoidGameState.h"
#include "Framework/ArkanoidPlayerController.h"
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

void AArkanoidGameMode::GameEnded(const bool bIsWinner)
{
	auto ArkanoidGameState = Cast<AArkanoidGameState>(GameState);
	if (ArkanoidGameState)
	{
		ArkanoidGameState->StopGame();
	}

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (PlayerState)
		{
			const auto Player = Cast<AArkanoidPlayerController>(PlayerState->GetPlayerController());
			if (Player)
			{
				Player->ShowGameEndMenu(bIsWinner);
			}
		}
	}
}
