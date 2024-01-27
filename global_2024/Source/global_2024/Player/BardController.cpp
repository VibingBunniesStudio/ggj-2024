// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BardController.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../Input/InputBindingsDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "InputCombo/InputComboManager.h"

void ABardController::BeginPlay()
{
	Super::BeginPlay();
	m_comboManager = Cast<AInputComboManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AInputComboManager::StaticClass()));

}

void ABardController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAxis(TEXT("MouseAxis"), this, &AVibingPlayerController::MouseAxis);
	// Make sure that we are using a UEnhancedInputComponent; if not, the project is not configured correctly.
	if (UEnhancedInputComponent* EnhancedInputCmp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		const UInputBindingsDeveloperSettings* inputSettings = GetDefault<UInputBindingsDeveloperSettings>();
		if (inputSettings == nullptr)
		{
			return;
		}
		EnhancedInputCmp->BindAction(inputSettings->GetInputActionByName("a"), ETriggerEvent::Started, this, &ABardController::ComboInputPressed, 0);
		EnhancedInputCmp->BindAction(inputSettings->GetInputActionByName("b"), ETriggerEvent::Started, this, &ABardController::ComboInputPressed, 1);
		EnhancedInputCmp->BindAction(inputSettings->GetInputActionByName("x"), ETriggerEvent::Started, this, &ABardController::ComboInputPressed, 2);
		EnhancedInputCmp->BindAction(inputSettings->GetInputActionByName("y"), ETriggerEvent::Started, this, &ABardController::ComboInputPressed, 3);
#if WITH_EDITOR
		EnhancedInputCmp->BindAction(inputSettings->GetInputActionByName("PauseEditor"), ETriggerEvent::Started, this, &ABardController::Pause);
#else
		EnhancedInputCmp->BindAction(inputSettings->GetInputActionByName("Pause"), ETriggerEvent::Started, this, &ABardController::Pause);
#endif
	}
}

void ABardController::Pause()
{
	onPause.Broadcast();
}

void ABardController::ComboInputPressed(int32 index)
{
	if (!m_comboManager)
	{
		return;
	}
	int32 playerId = UGameplayStatics::GetPlayerControllerID(this);
	if (m_comboManager->ComboInputPressed(playerId, index))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Good"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Wrong"));
	}

}
