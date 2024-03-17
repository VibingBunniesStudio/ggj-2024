// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "CustomSlider.generated.h"

/**
 * 
 */
UCLASS()
class GLOBAL_2024_API UCustomSlider : public USlider
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	void SetValueWithCallback(float InValue);
};
