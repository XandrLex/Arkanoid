// Alexandr Sharizanov. All rights reserved.


#include "Bonuses/StickyBallBonus.h"
#include "World/Paddle.h"

AStickyBallBonus::AStickyBallBonus()
{

}

void AStickyBallBonus::BonusAction(APaddle* Paddle)
{
	// Включаем режим прилипшего шарика на каретке на время Duration
	if (IsValid(Paddle))
	{
		Paddle->BonusStickyBall(Duration);
	}
	Destroy();
}

