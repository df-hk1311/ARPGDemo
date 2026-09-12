#pragma once

#include "CoreMinimal.h"
#include "Variant_Combat/CombatGameMode.h"
#include "ARPGArenaGameMode.generated.h"

class UARPGFlowWidget;

UCLASS()
class ARPG_API AARPGArenaGameMode : public ACombatGameMode
{
	GENERATED_BODY()

public:
	AARPGArenaGameMode();

	void EndMatch(bool bVictory);

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	void CreateFlowWidget(APlayerController* NewPlayer);

	UPROPERTY()
	TObjectPtr<UARPGFlowWidget> FlowWidget;
};