// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class GLOBAL_2024_API USoundManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true", DisplayName = "BGM Volume"))
	float _bgmVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true", DisplayName = "SFX Volume"))
	float _sfxVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true", DisplayName = "Voice Volume"))
	float _voiceVolume;

public:
	// Sets default values for this actor's properties
	USoundManager();

	UFUNCTION(BlueprintCallable)
	float GetBGMusicVolume();

	UFUNCTION(BlueprintCallable)
	bool SetBGMusicVolume(float newVolume);

	UFUNCTION(BlueprintCallable)
	float GetSFXVolume();

	UFUNCTION(BlueprintCallable)
	bool SetSFXVolume(float newVolume);

	UFUNCTION(BlueprintCallable)
	float GetVoiceVolume();

	UFUNCTION(BlueprintCallable)
	bool SetVoiceVolume(float newVolume);
	
};
