// Alexandr Sharizanov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bonuses/BonusParent.h"
#include "World/Paddle.h"
#include "BonusLife.generated.h"


UCLASS()
class ARKANOID_API ABonusLife : public ABonusParent
{
	GENERATED_BODY()
	
public:

	ABonusLife();

protected:

	virtual void BonusAction(APaddle* Paddle) override;
};
