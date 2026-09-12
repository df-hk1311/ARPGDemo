#pragma once

#include "CoreMinimal.h"
#include "ARPGFlowTypes.generated.h"

UENUM(BlueprintType)
enum class EARPGFlowState : uint8
{
	Boot UMETA(DisplayName = "Boot"),
	MainMenu UMETA(DisplayName = "Main Menu"),
	Playing UMETA(DisplayName = "Playing"),
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat")
};