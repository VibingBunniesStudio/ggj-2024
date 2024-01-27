// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundManager.generated.h"

class AInputComboManager;

UCLASS()
class GLOBAL_2024_API ARoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundManager();

	UFUNCTION(BlueprintCallable)
	void StartNewRound();

	void ApplyRoundResult(int winnerPlayerIndex);
	void FinishCombat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AInputComboManager* m_comboManager;
	int32 m_currentRound = -1;

};
