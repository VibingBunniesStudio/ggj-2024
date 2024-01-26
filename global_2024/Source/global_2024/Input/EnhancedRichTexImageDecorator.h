#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "CommonInputBaseTypes.h"
#include "Widgets/Layout/SScaleBox.h"
#include "EnhancedRichTexImageDecorator.generated.h"


class ISlateStyle;
class UCommonInputSubsystem;
class UInputAction;
class SHorizontalBox;


DECLARE_MULTICAST_DELEGATE(FDecoratorDestroyed)


class FEnhancedRichInlineImage : public ITextDecorator
{
public:
	FEnhancedRichInlineImage(URichTextBlock* InOwner, UCommonInputSubsystem* InInputSubsystem, UEnhancedRichTexImageDecorator* InDecorator)
	    : Owner(InOwner), CommonInputSubsystem(InInputSubsystem), Decorator(InDecorator), savedTextStyle(FTextBlockStyle::GetDefault()), savedStretch(EStretch::None)
	{
	}

	virtual ~FEnhancedRichInlineImage() override;

	/** Override this function to specify which types of tags are handled by this decorator */
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const;

	virtual TSharedRef<ISlateRun> Create(const TSharedRef<class FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText, const ISlateStyle* Style) override final;

protected:
	/** Override this function if you want to dynamically generate text, optionally changing the style. InOutString will start as the content between tags */
	virtual void CreateDecoratorText(const FTextRunInfo& RunInfo, FTextBlockStyle& InOutTextStyle, FString& InOutString) const;

	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle);

	void OnInputTypeChangedCallback(ECommonInputType inputType);

	void AddWidgetSlot(FName TagOrId, TSharedPtr<SHorizontalBox> containerWidget, FSlateBrush* Brush, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch);

	void UpdateRichInlineImage();

private:
	//void CreateContainerWidgetSlots(TArray<FSlateBrush*> Brushes, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch);
	void CreateContainerWidgetSlots(FName TagOrId, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch);

	void CleanUpDelegates();

protected:
	UPROPERTY()
	URichTextBlock* Owner;

	UPROPERTY()
	UCommonInputSubsystem* CommonInputSubsystem;

private:
	UPROPERTY()
	UEnhancedRichTexImageDecorator* Decorator;
	TArray<FName> savedTagsOrIds;
	TOptional<int32> savedWidth;
	TOptional<int32> savedHeight;
	FTextBlockStyle savedTextStyle;
	EStretch::Type savedStretch;
	TMap<FName, TArray<FSlateBrush>> savedBrushes;
	TMap < FName, TSharedPtr<SHorizontalBox>> savedContainerWidgets;
	TMap < FName, const FSlateBrush*> actionBrushes;
	FText alternativeBindSeparator;
	bool useAlternativeBindSeparator;

	UPROPERTY()
	FDelegateHandle inputMethodChangeHandle;

	UPROPERTY()
	FDelegateHandle inputMappingChangeHandle;

	UPROPERTY()
	FDelegateHandle decoratorDestroyedHandle;
};

UCLASS()
class GLOBAL_2024_API UEnhancedRichTexImageDecorator : public URichTextBlockImageDecorator
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	const TArray<FKey> FindInputActionKeys(FName TagOrId, bool bWarnIfMissing, const UInputAction* &outAction);
	class UCommonInputSubsystem* GetCommonInputSubsystem();
	class UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();
	virtual void BeginDestroy() override;

public:
	FDecoratorDestroyed onDecoratorDestroyed;
};
