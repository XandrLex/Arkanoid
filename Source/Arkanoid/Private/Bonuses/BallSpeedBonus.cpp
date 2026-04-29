// Alexandr Sharizanov. All rights reserved.


#include "Bonuses/BallSpeedBonus.h"
#include "World/Paddle.h"

ABallSpeedBonus::ABallSpeedBonus()
{
	Value = -0.5f;
}

void ABallSpeedBonus::BonusAction(APaddle* Paddle)
{
	Paddle->BonusChangeBallSpeed(Value);
	Super::BonusAction(Paddle);
}
