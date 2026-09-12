#include "Flow/ARPGFlowSubsystem.h"

#include "ARPG.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"

void UARPGFlowSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentState = EARPGFlowState::Boot;
}

bool UARPGFlowSubsystem::IsTransitionAllowed(const EARPGFlowState From, const EARPGFlowState To)
{
	switch (From)
	{
	case EARPGFlowState::Boot:
		return To == EARPGFlowState::MainMenu || To == EARPGFlowState::Playing;

	case EARPGFlowState::MainMenu:
		return To == EARPGFlowState::Playing;

	case EARPGFlowState::Playing:
		return To == EARPGFlowState::Victory
			|| To == EARPGFlowState::Defeat
			|| To == EARPGFlowState::MainMenu;

	case EARPGFlowState::Victory:
	case EARPGFlowState::Defeat:
		return To == EARPGFlowState::Playing || To == EARPGFlowState::MainMenu;

	default:
		return false;
	}
}

bool UARPGFlowSubsystem::SetFlowState(const EARPGFlowState NewState)
{
	if (CurrentState == NewState)
	{
		return true;
	}

	if (!IsTransitionAllowed(CurrentState, NewState))
	{
		UE_LOG(LogARPG, Warning, TEXT("Rejected flow transition: %s -> %s"),
			*UEnum::GetValueAsString(CurrentState),
			*UEnum::GetValueAsString(NewState));
		return false;
	}

	CurrentState = NewState;
	OnFlowStateChanged.Broadcast();
	return true;
}

void UARPGFlowSubsystem::StartNewGame(const UObject* WorldContextObject)
{
	if (SetFlowState(EARPGFlowState::Playing))
	{
		OpenLevel(WorldContextObject, GetArenaLevelName(), TEXT("?game=/Script/ARPG.ARPGArenaGameMode"));
	}
}

void UARPGFlowSubsystem::RestartMatch(const UObject* WorldContextObject)
{
	StartNewGame(WorldContextObject);
}

void UARPGFlowSubsystem::ReturnToMainMenu(const UObject* WorldContextObject)
{
	if (SetFlowState(EARPGFlowState::MainMenu))
	{
		OpenLevel(WorldContextObject, GetMainMenuLevelName(), TEXT("?game=/Script/ARPG.ARPGMenuGameMode"));
	}
}

void UARPGFlowSubsystem::FinishMatch(const bool bVictory)
{
	SetFlowState(bVictory ? EARPGFlowState::Victory : EARPGFlowState::Defeat);
}

FName UARPGFlowSubsystem::GetMainMenuLevelName()
{
	return FName(TEXT("/Game/ARPG/Flow/L_MainMenu"));
}

FName UARPGFlowSubsystem::GetArenaLevelName()
{
	return FName(TEXT("/Game/ARPG/Flow/L_Arena"));
}

void UARPGFlowSubsystem::OpenLevel(const UObject* WorldContextObject, const FName LevelName, const FString& Options) const
{
	if (!WorldContextObject)
	{
		UE_LOG(LogARPG, Error, TEXT("Cannot open level %s without a valid world context."), *LevelName.ToString());
		return;
	}

	UGameplayStatics::OpenLevel(WorldContextObject, LevelName, true, Options);
}

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FARPGFlowTransitionTest, "ARPG.Flow.Transitions",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FARPGFlowTransitionTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Boot can enter the main menu"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::Boot, EARPGFlowState::MainMenu));
	TestTrue(TEXT("Boot can enter the arena"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::Boot, EARPGFlowState::Playing));
	TestTrue(TEXT("Main menu can start a match"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::MainMenu, EARPGFlowState::Playing));
	TestTrue(TEXT("Playing can result in victory"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::Playing, EARPGFlowState::Victory));
	TestTrue(TEXT("Playing can result in defeat"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::Playing, EARPGFlowState::Defeat));
	TestTrue(TEXT("Victory can restart"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::Victory, EARPGFlowState::Playing));
	TestTrue(TEXT("Defeat can restart"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::Defeat, EARPGFlowState::Playing));
	TestFalse(TEXT("Main menu cannot jump directly to victory"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::MainMenu, EARPGFlowState::Victory));
	TestFalse(TEXT("Victory cannot become defeat"), UARPGFlowSubsystem::IsTransitionAllowed(EARPGFlowState::Victory, EARPGFlowState::Defeat));
	return true;
}
#endif