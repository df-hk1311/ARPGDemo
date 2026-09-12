#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGMenuGameMode.generated.h"

class UARPGFlowWidget;

UCLASS()
class ARPG_API AARPGMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARPGMenuGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	void CreateFlowWidget(APlayerController* NewPlayer);

	UPROPERTY()
	TObjectPtr<UARPGFlowWidget> FlowWidget;
};