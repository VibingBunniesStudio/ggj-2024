// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Managers/SoundManager.h"
#include "global_2024GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GLOBAL_2024_API Uglobal_2024GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX & Music")
	USoundManager* _soundManager;

public:
	Uglobal_2024GameInstance();
	
};
