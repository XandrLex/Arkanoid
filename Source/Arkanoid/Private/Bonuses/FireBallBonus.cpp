// Alexandr Sharizanov. All rights reserved.


#include "Bonuses/FireBallBonus.h"
#include "World/Ball.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AFireBallBonus::BonusAction(APaddle* /*Paddle*/)
{
	// Находим все шарики в мире и включаем им режим огненного шарика.
	if (!GetWorld())
	{
		Destroy();
		return;
	}

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), Found);

	for (AActor* Actor : Found)
	{
		if (ABall* Ball = Cast<ABall>(Actor))
		{
			// Duration берём из родительского поля Duration
			Ball->EnableFireBall(Duration);
		}
	}

	// Уничтожаем бонус
	Destroy();
}
