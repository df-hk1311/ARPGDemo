#include "Flow/ARPGMenuGameMode.h"

#include "Flow/ARPGFlowSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "UI/ARPGFlowWidget.h"

AARPGMenuGameMode::AARPGMenuGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerController::StaticClass();
	bStartPlayersAsSpectators = true;
}

void AARPGMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UARPGFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UARPGFlowSubsystem>())
	{
		FlowSubsystem->SetFlowState(EARPGFlowState::MainMenu);
	}
}

void AARPGMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	CreateFlowWidget(NewPlayer);
}

void AARPGMenuGameMode::CreateFlowWidget(APlayerController* NewPlayer)
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