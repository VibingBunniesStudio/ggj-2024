// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ExtendedCommonButtonBase.generated.h"

/**
 * 
 */
UCLASS()
class GLOBAL_2024_API UExtendedCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	
};
