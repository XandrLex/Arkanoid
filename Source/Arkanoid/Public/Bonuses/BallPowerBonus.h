// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bonuses/BonusParent.h"
#include "BallPowerBonus.generated.h"


UCLASS()
class ARKANOID_API ABallPowerBonus : public ABonusParent
{
	GENERATED_BODY()

public:

	ABallPowerBonus();

protected:

	virtual void BonusAction(APaddle* Paddle) override;
	
};
