// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "InputCombo/InputComboManager.h"

// Sets default values
ARoundManager::ARoundManager() : m_currentRound(-1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARoundManager::StartNewRound()
{
	m_currentRound++;
	if (m_comboManager)
	{
		m_comboManager->GenerateComboSecuence(5, 4);
	}
}

void ARoundManager::ApplyRoundResult(int winnerPlayerIndex)
{
}

void ARoundManager::FinishCombat()
{
}

// Called when the game starts or when spawned
void ARoundManager::BeginPlay()
{
	Super::BeginPlay();
	m_comboManager = Cast<AInputComboManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AInputComboManager::StaticClass()));
	
}

// Called every frame
void ARoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

