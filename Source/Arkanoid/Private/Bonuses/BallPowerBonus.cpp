// Alexandr Sharizanov. All rights reserved.


#include "Bonuses/BallPowerBonus.h"
#include "World/Paddle.h"

ABallPowerBonus::ABallPowerBonus()
{
	Value = 1.0f;
	Duration = 10.0f;
}

void ABallPowerBonus::BonusAction(APaddle* Paddle)
{
	Paddle->BonusChangeBallPower(Value, Duration);
	Super::BonusAction(Paddle);
}
