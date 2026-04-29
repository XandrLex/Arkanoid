// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bonuses/BonusParent.h"
#include "BallSpeedBonus.generated.h"


UCLASS()
class ARKANOID_API ABallSpeedBonus : public ABonusParent
{
	GENERATED_BODY()

public:

	ABallSpeedBonus();

protected:

	virtual void BonusAction(APaddle* Paddle) override;
	
};
