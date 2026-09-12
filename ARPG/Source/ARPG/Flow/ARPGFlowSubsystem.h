#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Flow/ARPGFlowTypes.h"
#include "ARPGFlowSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FARPGFlowStateChanged);

UCLASS()
class ARPG_API UARPGFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	bool SetFlowState(EARPGFlowState NewState);
	void StartNewGame(const UObject* WorldContextObject);
	void RestartMatch(const UObject* WorldContextObject);
	void ReturnToMainMenu(const UObject* WorldContextObject);
	void FinishMatch(bool bVictory);

	static bool IsTransitionAllowed(EARPGFlowState From, EARPGFlowState To);
	static FName GetMainMenuLevelName();
	static FName GetArenaLevelName();

	EARPGFlowState GetFlowState() const { return CurrentState; }

	FARPGFlowStateChanged OnFlowStateChanged;

private:
	void OpenLevel(const UObject* WorldContextObject, FName LevelName, const FString& Options) const;

	EARPGFlowState CurrentState = EARPGFlowState::Boot;
};