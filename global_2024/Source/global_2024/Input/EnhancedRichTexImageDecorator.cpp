

#include "Input/EnhancedRichTexImageDecorator.h"
#include "CommonInputSubsystem.h"
#include <EnhancedActionKeyMapping.h>
#include "Input/InputBindingsDeveloperSettings.h"
#include <Kismet/GameplayStatics.h>
#include "Components/RichTextBlock.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputDeveloperSettings.h"
#include "EnhancedInputSubsystemHelpers.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Text/SlateTextRun.h"
#include "Framework/Text/SlateWidgetRun.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DefaultValueHelper.h"
#include "UObject/UObjectGlobals.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#define DEFAULT_ALTERNATIVE_SEPARATOR " /"
#define DEFAULT_CHORDED_SEPARATOR " +"



FEnhancedRichInlineImage::~FEnhancedRichInlineImage()
{
	CleanUpDelegates();
	Decorator = nullptr;
}

bool FEnhancedRichInlineImage::Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const
{
	if (RunParseResult.Name == TEXT("input") && RunParseResult.MetaData.Contains(TEXT("id")))
	{
		const FTextRange& IdRange = RunParseResult.MetaData[TEXT("id")];
		const FString TagId = Text.Mid(IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex);

		const bool bWarnIfMissing = false;
		const UInputAction* outAction;
		return Decorator->FindInputActionKeys(*TagId, bWarnIfMissing, outAction).Num() > 0;
	}

	return false;
}

TSharedRef<ISlateRun> FEnhancedRichInlineImage::Create(const TSharedRef<class FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText, const ISlateStyle* Style)
{
	FTextRange ModelRange;
	ModelRange.BeginIndex = InOutModelText->Len();

	FTextRunInfo RunInfo(RunParseResult.Name, FText::FromString(OriginalText.Mid(RunParseResult.ContentRange.BeginIndex, RunParseResult.ContentRange.EndIndex - RunParseResult.ContentRange.BeginIndex)));
	for (const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}

	const FTextBlockStyle& TextStyle = Owner->GetCurrentDefaultTextStyle();

	TSharedPtr<ISlateRun> SlateRun;
	TSharedPtr<SWidget> DecoratorWidget = CreateDecoratorWidget(RunInfo, TextStyle);
	if (DecoratorWidget.IsValid())
	{
		*InOutModelText += TEXT('\u200B'); // Zero-Width Breaking Space
		ModelRange.EndIndex = InOutModelText->Len();

		// Calculate the baseline of the text within the owning rich text
		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		int16 WidgetBaseline = FontMeasure->GetBaseline(TextStyle.Font) - FMath::Min(0.0f, TextStyle.ShadowOffset.Y);

		FSlateWidgetRun::FWidgetRunInfo WidgetRunInfo(DecoratorWidget.ToSharedRef(), WidgetBaseline);
		SlateRun = FSlateWidgetRun::Create(TextLayout, RunInfo, InOutModelText, WidgetRunInfo, ModelRange);
	}
	else
	{
		// Assume there's a text handler if widget is empty, if there isn't one it will just display an empty string
		FTextBlockStyle TempStyle = TextStyle;
		CreateDecoratorText(RunInfo, TempStyle, *InOutModelText);

		ModelRange.EndIndex = InOutModelText->Len();
		SlateRun = FSlateTextRun::Create(RunInfo, InOutModelText, TempStyle, ModelRange);
	}

	return SlateRun.ToSharedRef();
}

void FEnhancedRichInlineImage::CreateDecoratorText(const FTextRunInfo& RunInfo, FTextBlockStyle& InOutTextStyle, FString& InOutString) const
{
}

TSharedPtr<SWidget> FEnhancedRichInlineImage::CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle)
{
	FString action = "";
	const FSlateBrush* actionBrush = nullptr;
	if (RunInfo.MetaData.Contains(TEXT("action")))
	{
		action = RunInfo.MetaData[TEXT("action")];
		actionBrush = Decorator->FindImageBrush(*action, true);
	}

	FString id = RunInfo.MetaData[TEXT("id")] + "." + action;
	FName tagOrId = *id;
	if (actionBrush != nullptr)
	{
		actionBrushes.Add(tagOrId, actionBrush);
	}
	if (savedContainerWidgets.Contains(tagOrId))
	{
		return *savedContainerWidgets.Find(tagOrId);
	}
	savedTagsOrIds.Add(tagOrId);
	savedBrushes.Add(tagOrId, TArray<FSlateBrush>());
	alternativeBindSeparator = FText::FromString(DEFAULT_ALTERNATIVE_SEPARATOR);
	if (RunInfo.MetaData.Contains(TEXT("separator")))
	{
		alternativeBindSeparator = FText::FromString(*RunInfo.MetaData[TEXT("separator")]);
	}
	useAlternativeBindSeparator = !RunInfo.MetaData.Contains(TEXT("noSeparator"));

	TOptional<int32> Width;
	if (const FString* WidthString = RunInfo.MetaData.Find(TEXT("width")))
	{
		int32 WidthTemp;
		Width = FDefaultValueHelper::ParseInt(*WidthString, WidthTemp) ? WidthTemp : TOptional<int32>();
	}

	TOptional<int32> Height;
	if (const FString* HeightString = RunInfo.MetaData.Find(TEXT("height")))
	{
		int32 HeightTemp;
		Height = FDefaultValueHelper::ParseInt(*HeightString, HeightTemp) ? HeightTemp : TOptional<int32>();
	}

	EStretch::Type Stretch = EStretch::ScaleToFit;
	if (const FString* SstretchString = RunInfo.MetaData.Find(TEXT("stretch")))
	{
		static const UEnum* StretchEnum = StaticEnum<EStretch::Type>();
		int64 StretchValue = StretchEnum->GetValueByNameString(*SstretchString);
		if (StretchValue != INDEX_NONE)
		{
			Stretch = static_cast<EStretch::Type>(StretchValue);
		}
	}

	if (Decorator != nullptr)
	{
		if (CommonInputSubsystem != nullptr && !CommonInputSubsystem->OnInputMethodChangedNative.IsBoundToObject(this))
		{
			inputMethodChangeHandle = CommonInputSubsystem->OnInputMethodChangedNative.AddRaw(this, &FEnhancedRichInlineImage::OnInputTypeChangedCallback);
		}
		if (!UEnhancedInputSubsystemHelpers::onInputMappingsChanged.IsBoundToObject(this))
		{
			inputMappingChangeHandle = UEnhancedInputSubsystemHelpers::onInputMappingsChanged.AddRaw(this, &FEnhancedRichInlineImage::UpdateRichInlineImage);
		}
		if (!Decorator->onDecoratorDestroyed.IsBoundToObject(this))
		{
			decoratorDestroyedHandle = Decorator->onDecoratorDestroyed.AddRaw(this, &FEnhancedRichInlineImage::CleanUpDelegates);
		}
	}
	TSharedPtr<SHorizontalBox> containerWidget = SNew(SHorizontalBox);
	savedContainerWidgets.Add(tagOrId, containerWidget);
	savedTextStyle = TextStyle;
	savedWidth = Width;
	savedHeight = Height;
	savedStretch = Stretch;

	CreateContainerWidgetSlots(tagOrId, TextStyle, Width, Height, Stretch);

	return containerWidget;
}

void FEnhancedRichInlineImage::OnInputTypeChangedCallback(ECommonInputType inputType)
{
	UpdateRichInlineImage();
}


void FEnhancedRichInlineImage::UpdateRichInlineImage()
{
	if (ensure(Decorator))
	{
		for (FName tagOrId: savedTagsOrIds)
		{
			TSharedPtr<SHorizontalBox> containerWidget = savedContainerWidgets.FindRef(tagOrId);
			if (ensure(containerWidget))
			{
				// Only one reference means this is the only component that is using the widget
				// This means it is no longer being rendered
				if (containerWidget.GetSharedReferenceCount() <= 1)
				{
					savedContainerWidgets.Remove(tagOrId);
					actionBrushes.Remove(tagOrId);
				}
				else
				{
					containerWidget->ClearChildren();
					CreateContainerWidgetSlots(tagOrId, savedTextStyle, savedWidth, savedHeight, savedStretch);
					if (Owner && Owner->GetParent() != nullptr && Owner->GetCachedWidget() && Owner->GetVisibility() == ESlateVisibility::Visible)
					{
						Owner->TakeWidget();
						Owner->InvalidateLayoutAndVolatility();
						Owner->SynchronizeProperties();
					}
				}
			}
		}
	}
}

void FEnhancedRichInlineImage::AddWidgetSlot(FName TagOrId, TSharedPtr<SHorizontalBox> containerWidget, FSlateBrush* Brush, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch)
{
	int index = 0;
	if (ensure(containerWidget) && ensure(Brush))
	{
		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		float IconHeight = FMath::Min((float)FontMeasure->GetMaxCharacterHeight(TextStyle.Font, 1.0f), Brush->ImageSize.Y);
		float IconWidth = IconHeight;

		if (Width.IsSet())
		{
			IconWidth = Width.GetValue();
		}

		if (Height.IsSet())
		{
			IconHeight = Height.GetValue();
		}
		containerWidget->AddSlot();
		SHorizontalBox::FSlot& slot = containerWidget->GetSlot(containerWidget->NumSlots() - 1);

		const FSlateBrush* actionBrush = actionBrushes.FindRef(TagOrId);

		if (actionBrush != nullptr)
		{
			/*TSharedPtr<SOverlay> overlayWidget = SNew(SOverlay);
			overlayWidget->AddSlot();
			SOverlay::FOverlaySlot& overlaySlot = overlayWidget->GetSlot();
			overlaySlot.AttachWidget(SNew(SBox)
			                      .HeightOverride(IconHeight)
			                      .WidthOverride(IconWidth)
			                          [SNew(SScaleBox)
			                                  .Stretch(Stretch)
			                                  .StretchDirection(EStretchDirection::DownOnly)
			                                  .VAlign(VAlign_Center)
			                                      [SNew(SImage)
			                                                     .Image(Brush)]]);
			SOverlay::FOverlaySlot& overlayActionSlot = overlayWidget->AddSlot();
			overlayActionSlot.AttachWidget(SNew(SBox)
			                       .HeightOverride(IconHeight)
			                       .WidthOverride(IconWidth)
			                           [SNew(SScaleBox)
			                                   .Stretch(Stretch)
			                                   .StretchDirection(EStretchDirection::DownOnly)
			                                   .VAlign(VAlign_Center)
			                                       [SNew(SImage)
			                                               .Image(actionBrush)]]);*/
			slot.AttachWidget(SNew(SBox)
				.HeightOverride(IconHeight)
				.WidthOverride(IconWidth)
				[SNew(SScaleBox)
				.Stretch(Stretch)
				.StretchDirection(EStretchDirection::DownOnly)
				.VAlign(VAlign_Center)
				[SNew(SImage)
				.Image(actionBrush)]]);
		}
		else
		{
			slot.AttachWidget(SNew(SBox)
			                      .HeightOverride(IconHeight)
			                      .WidthOverride(IconWidth)
			                          [SNew(SScaleBox)
			                                  .Stretch(Stretch)
			                                  .StretchDirection(EStretchDirection::DownOnly)
			                                  .VAlign(VAlign_Center)
			                                      [SNew(SImage)
			                                              .Image(Brush)]]);

		}
	}
}

void FEnhancedRichInlineImage::CreateContainerWidgetSlots(FName TagOrId, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch)
{
	CommonInputSubsystem = Decorator->GetCommonInputSubsystem();
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = Decorator->GetEnhancedInputLocalPlayerSubsystem();

	TSharedPtr<SHorizontalBox> containerWidget = savedContainerWidgets.FindRef(TagOrId);
	if (enhancedInputSubsystem == nullptr || CommonInputSubsystem == nullptr || containerWidget == nullptr)
	{
		return;
	}
	const UInputAction* outAction;
	FString inputAction;
	TagOrId.ToString().Split(".", &inputAction, nullptr);
	TArray<FKey> keys = Decorator->FindInputActionKeys(*inputAction, true, outAction);
	TArray<FSlateBrush>* brushes = savedBrushes.Find(TagOrId);
	brushes->Empty();
	
	//UProjectEitrGameUserSettings* gameUserSettings = UProjectEitrGameUserSettings::GetUserSettings();

	int slotCount = 0;
	for (const FKey& key : keys)
	{
		FSlateBrush slateBrush;
		
		
		// Chorded Action Key

		/*if (gameUserSettings != nullptr)
		{
			TArray<FEnhancedActionKeyMapping>::SizeType MappingIdx = gameUserSettings->GetDefaultMappingContext()->GetMappings().IndexOfByPredicate(
			    [&outAction, &key](const FEnhancedActionKeyMapping& Mapping)
			    {
				    return Mapping.Action->GetFName() == outAction->GetFName() && Mapping.Key == key;
			    });
			if (MappingIdx >= 0)
			{
				const FEnhancedActionKeyMapping mapping = gameUserSettings->GetDefaultMappingContext()->GetMapping(MappingIdx);
				for (int i = 0; i < mapping.Triggers.Num(); i++)
				{
					if (UInputTriggerChordAction* chordedAction = Cast<UInputTriggerChordAction>(mapping.Triggers[i]))
					{
						TArray<FKey> MappedKeys;

						TArray<FKey> chordedActionKeys = enhancedInputSubsystem->QueryKeysMappedToAction(chordedAction->ChordAction);
						if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(slateBrush, chordedActionKeys, CommonInputSubsystem->GetCurrentInputType(), CommonInputSubsystem->GetCurrentGamepadName()))
						{
							int index = brushes->Add(slateBrush);
							AddWidgetSlot(TagOrId, containerWidget, &(*brushes)[index], TextStyle, Width, Height, Stretch);
							SBoxPanel::FSlot& slashSlot = savedContainerWidgets.FindRef(TagOrId)->AddSlot();
							slashSlot.AttachWidget(SNew(STextBlock)
							                           .TextStyle(&TextStyle)
							                           .Text(FText::FromString(DEFAULT_CHORDED_SEPARATOR)));
						}
					}
				}
			}
		}*/

		// Main key
		if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(slateBrush, key, CommonInputSubsystem->GetCurrentInputType(), CommonInputSubsystem->GetCurrentGamepadName()))
		{
			int index = brushes->Add(slateBrush);
			AddWidgetSlot(TagOrId, containerWidget, &(*brushes)[index], TextStyle, Width, Height, Stretch);
		}

		// Alternative keys
		if (useAlternativeBindSeparator && keys.Num() > 1 && slotCount < keys.Num() - 1)
		{

			SHorizontalBox::FSlot& slashSlot = containerWidget->GetSlot(containerWidget->NumSlots() - 1);
			slashSlot.AttachWidget(SNew(STextBlock)
			                           .TextStyle(&TextStyle)
			                           .Text(alternativeBindSeparator));
		}
		++slotCount;
	}
}

void FEnhancedRichInlineImage::CleanUpDelegates()
{
	if (Decorator != nullptr)
	{
		if (CommonInputSubsystem != nullptr)
		{
			CommonInputSubsystem->OnInputMethodChangedNative.Remove(inputMethodChangeHandle);
		}

		{
			UEnhancedInputSubsystemHelpers::onInputMappingsChanged.Remove(inputMappingChangeHandle);
		}

		Decorator->onDecoratorDestroyed.Remove(decoratorDestroyedHandle);
	}
}

TSharedPtr<ITextDecorator> UEnhancedRichTexImageDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	// This new implementation is different from the parent class. Intentionally not calling super
	APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UCommonInputSubsystem* commonInputSubsystem = playerController ? ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(playerController->GetLocalPlayer()) : nullptr;
	FEnhancedRichInlineImage* image = new FEnhancedRichInlineImage(InOwner, commonInputSubsystem, this);
	TSharedPtr<ITextDecorator> decorator = MakeShareable(image);
	return decorator;
}

const TArray<FKey> UEnhancedRichTexImageDecorator::FindInputActionKeys(FName TagOrId, bool bWarnIfMissing, const UInputAction*& outAction)
{
	TArray<FSlateBrush*> brushes;
	TArray<FKey> keys;

	const UInputBindingsDeveloperSettings* inputSettings = GetDefault<UInputBindingsDeveloperSettings>();
	if (inputSettings == nullptr)
	{
		return keys;
	}
	UCommonInputSubsystem* commonInputSubsystem = GetCommonInputSubsystem();
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = GetEnhancedInputLocalPlayerSubsystem();
	if (enhancedInputSubsystem == nullptr || commonInputSubsystem == nullptr)
	{
		return keys;
	}
	const UInputAction* action = inputSettings->GetInputActionByName(TagOrId);
	outAction = action;
	keys = enhancedInputSubsystem->QueryKeysMappedToAction(action);
	keys = keys.FilterByPredicate([&commonInputSubsystem](const FKey& key) -> bool
	    { return (key.IsGamepadKey() && commonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad) ||
		(!key.IsGamepadKey() && commonInputSubsystem->GetCurrentInputType() != ECommonInputType::Gamepad); });
	return keys;
}

UCommonInputSubsystem* UEnhancedRichTexImageDecorator::GetCommonInputSubsystem()
{
	APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (playerController == nullptr)
	{
		return nullptr;
	}
	return ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(playerController->GetLocalPlayer());
}

UEnhancedInputLocalPlayerSubsystem* UEnhancedRichTexImageDecorator::GetEnhancedInputLocalPlayerSubsystem()
{
	APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (playerController == nullptr)
	{
		return nullptr;
	}
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
}

void UEnhancedRichTexImageDecorator::BeginDestroy()
{
	//Maybe this order is not correct but whatever
	onDecoratorDestroyed.Broadcast();
	Super::BeginDestroy();
}
