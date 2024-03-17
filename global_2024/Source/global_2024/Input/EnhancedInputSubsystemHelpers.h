#pragma once

#include "CoreMinimal.h"
#include <EnhancedInputComponent.h>
#include "EnhancedInputDeveloperSettings.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "UObject/NoExportTypes.h"
#include <EnhancedInputLibrary.h>
#include "EnhancedInputSubsystemHelpers.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputMappingsChanged);
DECLARE_MULTICAST_DELEGATE(FInputMappingsChanged)

typedef void (*TCallbackFunction)(const FInputActionValue&);

UCLASS()
class GLOBAL_2024_API UEnhancedInputSubsystemHelpers : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void AddMappingContext(UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem, const UInputMappingContext* MappingContext, int32 Priority)
	{
		if(enhancedInputSubsystem)
		{
			enhancedInputSubsystem->AddMappingContext(MappingContext, Priority);
			UEnhancedInputLibrary::RequestRebuildControlMappingsUsingContext(MappingContext, true);
			onInputMappingsChanged.Broadcast();
		}
	}

	UFUNCTION()
	static void BroadCastInputMappingChanged()
	{
		onInputMappingsChanged.Broadcast();
	}

	static UEnhancedInputSubsystemHelpers* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = NewObject<UEnhancedInputSubsystemHelpers>();
		}
		return instance;
	}

	static void ListenToInputAction(UEnhancedInputComponent* EnhancedInputCmp, UInputAction* Action, UObject* Object, TCallbackFunction CallBackFunction)
	{
		const UEnhancedInputDeveloperSettings* inputSettings = GetDefault<UEnhancedInputDeveloperSettings>();
	}

public:
	static FInputMappingsChanged onInputMappingsChanged;
	static UEnhancedInputSubsystemHelpers* instance;
};
