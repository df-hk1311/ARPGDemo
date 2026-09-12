#include "Flow/ARPGArenaGameMode.h"

#include "ARPG.h"
#include "Flow/ARPGFlowSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "UI/ARPGFlowWidget.h"
#include "UObject/ConstructorHelpers.h"

AARPGArenaGameMode::AARPGArenaGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PawnClassFinder(TEXT("/Game/Variant_Combat/Blueprints/BP_CombatCharacter"));
	if (PawnClassFinder.Succeeded())
	{
		DefaultPawnClass = PawnClassFinder.Class;
	}
	else
	{
		UE_LOG(LogARPG, Warning, TEXT("Could not find BP_CombatCharacter; the arena will use the default pawn."));
	}

	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClassFinder(TEXT("/Game/Variant_Combat/Blueprints/BP_CombatPlayerController"));
	if (ControllerClassFinder.Succeeded())
	{
		PlayerControllerClass = ControllerClassFinder.Class;
	}
	else
	{
		UE_LOG(LogARPG, Warning, TEXT("Could not find BP_CombatPlayerController; the arena will use APlayerController."));
		PlayerControllerClass = APlayerController::StaticClass();
	}
}

void AARPGArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UARPGFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UARPGFlowSubsystem>())
	{
		FlowSubsystem->SetFlowState(EARPGFlowState::Playing);
	}
}

void AARPGArenaGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (UARPGFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UARPGFlowSubsystem>())
	{
		FlowSubsystem->SetFlowState(EARPGFlowState::Playing);
	}

	CreateFlowWidget(NewPlayer);
}

void AARPGArenaGameMode::CreateFlowWidget(APlayerController* NewPlayer)
{
	if (FlowWidget || !NewPlayer)
	{
		return;
	}

	FlowWidget = CreateWidget<UARPGFlowWidget>(NewPlayer, UARPGFlowWidget::StaticClass());
	if (FlowWidget)
	{
		FlowWidget->AddToPlayerScreen(10);
	}
}

void AARPGArenaGameMode::EndMatch(const bool bVictory)
{
	if (UARPGFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UARPGFlowSubsystem>())
	{
		FlowSubsystem->FinishMatch(bVictory);
	}
}