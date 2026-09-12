#include "UI/ARPGFlowWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Flow/ARPGArenaGameMode.h"
#include "Flow/ARPGFlowSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Styling/CoreStyle.h"

TSharedRef<SWidget> UARPGFlowWidget::RebuildWidget()
{
	BuildWidgetTree();
	return Super::RebuildWidget();
}

void UARPGFlowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);
	BuildWidgetTree();
}

void UARPGFlowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FlowSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UARPGFlowSubsystem>() : nullptr;

	if (FlowSubsystem)
	{
		FlowStateChangedHandle = FlowSubsystem->OnFlowStateChanged.AddUObject(this, &UARPGFlowWidget::RefreshContent);
	}

	RefreshContent();
}

void UARPGFlowWidget::NativeDestruct()
{
	if (FlowSubsystem && FlowStateChangedHandle.IsValid())
	{
		FlowSubsystem->OnFlowStateChanged.Remove(FlowStateChangedHandle);
	}

	FlowStateChangedHandle.Reset();
	Super::NativeDestruct();
}

void UARPGFlowWidget::BuildWidgetTree()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
	WidgetTree->RootWidget = Root;

	UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Background"));
	Background->SetBrushColor(FLinearColor(0.02f, 0.025f, 0.035f, 0.92f));
	Background->SetPadding(FMargin(36.0f, 30.0f));

	UCanvasPanelSlot* BackgroundSlot = Root->AddChildToCanvas(Background);
	BackgroundSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	BackgroundSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	BackgroundSlot->SetAutoSize(true);

	UVerticalBox* Content = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content"));
	Background->SetContent(Content);

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Title"));
	TitleText->SetJustification(ETextJustify::Center);
	TitleText->SetFont(FCoreStyle::GetDefaultFontStyle(TEXT("Bold"), 34));
	TitleText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	UVerticalBoxSlot* TitleSlot = Content->AddChildToVerticalBox(TitleText);
	TitleSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	TitleSlot->SetHorizontalAlignment(HAlign_Fill);

	StatusText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Status"));
	StatusText->SetJustification(ETextJustify::Center);
	StatusText->SetFont(FCoreStyle::GetDefaultFontStyle(TEXT("Regular"), 16));
	StatusText->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.78f, 0.85f)));
	UVerticalBoxSlot* StatusSlot = Content->AddChildToVerticalBox(StatusText);
	StatusSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 18.0f));
	StatusSlot->SetHorizontalAlignment(HAlign_Fill);

	ActionsBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Actions"));
	Content->AddChildToVerticalBox(ActionsBox);
}

void UARPGFlowWidget::RefreshContent()
{
	if (!FlowSubsystem)
	{
		return;
	}

	ActionsBox->ClearChildren();

	switch (FlowSubsystem->GetFlowState())
	{
	case EARPGFlowState::Boot:
	case EARPGFlowState::MainMenu:
		TitleText->SetText(FText::FromString(TEXT("ARPG \u6f14\u793a")));
		StatusText->SetText(FText::FromString(TEXT("\u6d41\u7a0b\u57fa\u7840\uff1a\u4e3b\u83dc\u5355")));
		AddActionButton(FText::FromString(TEXT("\u5f00\u59cb\u6e38\u620f")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleStartGame));
		AddActionButton(FText::FromString(TEXT("\u9000\u51fa\u6e38\u620f")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleQuit));
		break;

	case EARPGFlowState::Playing:
		TitleText->SetText(FText::FromString(TEXT("\u7ade\u6280\u573a\u6d41\u7a0b\u9aa8\u67b6")));
		StatusText->SetText(FText::FromString(TEXT("\u5f53\u524d\u4e3a\u4e34\u65f6\u8c03\u8bd5\u63a7\u5236\uff0c\u6218\u6597\u548c\u6ce2\u6b21\u7cfb\u7edf\u63a5\u5165\u540e\u5c06\u66ff\u6362\u3002")));
		AddActionButton(FText::FromString(TEXT("\u5f3a\u5236\u80dc\u5229")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleForceVictory));
		AddActionButton(FText::FromString(TEXT("\u5f3a\u5236\u5931\u8d25")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleForceDefeat));
		AddActionButton(FText::FromString(TEXT("\u8fd4\u56de\u4e3b\u83dc\u5355")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleReturnToMenu));
		break;

	case EARPGFlowState::Victory:
		TitleText->SetText(FText::FromString(TEXT("\u80dc\u5229")));
		StatusText->SetText(FText::FromString(TEXT("\u7ade\u6280\u573a\u6d41\u7a0b\u5df2\u6210\u529f\u5b8c\u6210\u3002")));
		AddActionButton(FText::FromString(TEXT("\u91cd\u65b0\u6311\u6218")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleRestartMatch));
		AddActionButton(FText::FromString(TEXT("\u4e3b\u83dc\u5355")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleReturnToMenu));
		break;

	case EARPGFlowState::Defeat:
		TitleText->SetText(FText::FromString(TEXT("\u5931\u8d25")));
		StatusText->SetText(FText::FromString(TEXT("\u672c\u6b21\u7ade\u6280\u573a\u6311\u6218\u5931\u8d25\u3002")));
		AddActionButton(FText::FromString(TEXT("\u91cd\u65b0\u6311\u6218")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleRestartMatch));
		AddActionButton(FText::FromString(TEXT("\u4e3b\u83dc\u5355")), GET_FUNCTION_NAME_CHECKED(UARPGFlowWidget, HandleReturnToMenu));
		break;

	default:
		break;
	}

	ApplyInputMode();
}

void UARPGFlowWidget::ApplyInputMode()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController || !FlowSubsystem)
	{
		return;
	}

	const bool bUseUIOnly = FlowSubsystem->GetFlowState() != EARPGFlowState::Playing;
	PlayerController->SetShowMouseCursor(true);

	if (bUseUIOnly)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetWidgetToFocus(TakeWidget());
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetWidgetToFocus(TakeWidget());
		PlayerController->SetInputMode(InputMode);
	}
}

void UARPGFlowWidget::AddActionButton(const FText& Label, const FName& HandlerName)
{
	if (!WidgetTree || !ActionsBox)
	{
		return;
	}

	UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());


	UTextBlock* LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	LabelText->SetText(Label);
	LabelText->SetJustification(ETextJustify::Center);
	LabelText->SetFont(FCoreStyle::GetDefaultFontStyle(TEXT("Regular"), 18));
	LabelText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	Button->AddChild(LabelText);

	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, HandlerName);
	Button->OnClicked.Add(Delegate);

	UVerticalBoxSlot* ButtonSlot = ActionsBox->AddChildToVerticalBox(Button);
	ButtonSlot->SetPadding(FMargin(0.0f, 7.0f));
	ButtonSlot->SetHorizontalAlignment(HAlign_Fill);
}

void UARPGFlowWidget::HandleStartGame()
{
	if (FlowSubsystem)
	{
		FlowSubsystem->StartNewGame(this);
	}
}

void UARPGFlowWidget::HandleRestartMatch()
{
	if (FlowSubsystem)
	{
		FlowSubsystem->RestartMatch(this);
	}
}

void UARPGFlowWidget::HandleReturnToMenu()
{
	if (FlowSubsystem)
	{
		FlowSubsystem->ReturnToMainMenu(this);
	}
}

void UARPGFlowWidget::HandleQuit()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UARPGFlowWidget::HandleForceVictory()
{
	if (AARPGArenaGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AARPGArenaGameMode>() : nullptr)
	{
		GameMode->EndMatch(true);
	}
}

void UARPGFlowWidget::HandleForceDefeat()
{
	if (AARPGArenaGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AARPGArenaGameMode>() : nullptr)
	{
		GameMode->EndMatch(false);
	}
}