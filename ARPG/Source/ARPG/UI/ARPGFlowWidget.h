#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Flow/ARPGFlowTypes.h"
#include "ARPGFlowWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;
class UARPGFlowSubsystem;

UCLASS()
class ARPG_API UARPGFlowWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BuildWidgetTree();
	void RefreshContent();
	void ApplyInputMode();
	void AddActionButton(const FText& Label, const FName& HandlerName);

	UFUNCTION()
	void HandleStartGame();

	UFUNCTION()
	void HandleRestartMatch();

	UFUNCTION()
	void HandleReturnToMenu();

	UFUNCTION()
	void HandleQuit();

	UFUNCTION()
	void HandleForceVictory();

	UFUNCTION()
	void HandleForceDefeat();

	UPROPERTY()
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY()
	TObjectPtr<UTextBlock> StatusText;

	UPROPERTY()
	TObjectPtr<UVerticalBox> ActionsBox;

	UPROPERTY()
	TObjectPtr<UARPGFlowSubsystem> FlowSubsystem;

	FDelegateHandle FlowStateChangedHandle;
};