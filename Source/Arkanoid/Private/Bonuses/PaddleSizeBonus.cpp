// Alexandr Sharizanov. All rights reserved.


#include "Bonuses/PaddleSizeBonus.h"

APaddleSizeBonus::APaddleSizeBonus()
{
	Duration = 10.0f;
}

void APaddleSizeBonus::BonusAction(APaddle* Paddle)
{
	Paddle->BonusChangeSize(Value, Duration);
	Super::BonusAction(Paddle);
}
