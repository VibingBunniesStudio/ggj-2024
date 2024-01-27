// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BardController.generated.h"

class AInputComboManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPause);

UCLASS()
class GLOBAL_2024_API ABardController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	void SetupInputComponent() override;
	void Pause();
	void ComboInputPressed(int32 index);

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FPause onPause;
	AInputComboManager* m_comboManager;
};
