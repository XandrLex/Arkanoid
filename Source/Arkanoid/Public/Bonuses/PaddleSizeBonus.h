// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bonuses/BonusParent.h"
#include "PaddleSizeBonus.generated.h"


UCLASS()
class ARKANOID_API APaddleSizeBonus : public ABonusParent
{
	GENERATED_BODY()
	
public:

	APaddleSizeBonus();

protected:

	virtual void BonusAction(APaddle* Paddle) override;

};
