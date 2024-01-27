// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundManager.generated.h"

class AInputComboManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerHealthChanged, int32, playerIndex, int32, remainingHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatEnd, int32, winnerPlayerIndex);

UCLASS()
class GLOBAL_2024_API ARoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartNewRound();

	void ApplyRoundResult(int winnerPlayerIndex);
	void FinishCombat();
	UFUNCTION()
	void EvaluatePlayerHealth(int32 playerIndex, int32 remainingHealth);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	AInputComboManager* m_comboManager;
	int32 m_currentRound = -1;
	UPROPERTY(BlueprintAssignable)
	FPlayerHealthChanged m_onHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FCombatEnd m_onCombatEnd;
};
