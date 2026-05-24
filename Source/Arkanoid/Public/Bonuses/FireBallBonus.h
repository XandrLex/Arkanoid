// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bonuses/BonusParent.h"
#include "FireBallBonus.generated.h"


UCLASS()
class ARKANOID_API AFireBallBonus : public ABonusParent
{
	GENERATED_BODY()
	
protected:
	// При подборе бонуса даём команде всем шарам мира включить режим огненного шарика на Duration (из родителя)
	virtual void BonusAction(APaddle* Paddle) override;
};
