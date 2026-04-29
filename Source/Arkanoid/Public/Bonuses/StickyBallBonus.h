// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bonuses/BonusParent.h"
#include "StickyBallBonus.generated.h"

UCLASS()
class ARKANOID_API AStickyBallBonus : public ABonusParent
{
	GENERATED_BODY()

public:
	
	AStickyBallBonus();

protected:
	
	virtual void BonusAction(APaddle* Paddle) override;
};
