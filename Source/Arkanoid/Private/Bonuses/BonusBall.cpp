// Alexandr Sharizanov. All rights reserved.


#include "Bonuses/BonusBall.h"
#include "World/Paddle.h"


ABonusBall::ABonusBall()
{
	Duration = 10.0f;
}

void ABonusBall::BonusAction(APaddle* Paddle)
{
	Paddle->BonusBall(Duration);

	Super::BonusAction(Paddle);
}
