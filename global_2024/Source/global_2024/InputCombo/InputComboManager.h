// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputComboManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FComboInputSuccess, int32, playerIndex, int32, comboProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComboInputFail, int32, playerIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComboSequenceComplete, int32, playerIndex);

UCLASS()
class GLOBAL_2024_API AInputComboManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInputComboManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetKeyComboIndex(FName key);

	bool ComboInputPressed(int32 playerIndex, int32 pressedKeyIndex);

	TMap<int32, int32> m_playerComboProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> m_comboSequence;

	UPROPERTY(BlueprintAssignable)
	FComboInputSuccess m_onComboInputSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FComboInputFail m_onComboInputFail;

	UPROPERTY(BlueprintAssignable)
	FComboSequenceComplete m_onComboSequenceComplete;


};
