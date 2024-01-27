// Fill out your copyright notice in the Description page of Project Settings.


#include "InputCombo/InputComboManager.h"

// Sets default values
AInputComboManager::AInputComboManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInputComboManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInputComboManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInputComboManager::GenerateComboSecuence(int32 numberOfInputs, int32 differentInputsAmounts)
{
	m_comboSequence.Empty();
	for (int i = 0; i < numberOfInputs; i++)
	{
		m_comboSequence.Add(GetComboIndexKey(FMath::RandRange(0, differentInputsAmounts - 1)));
	}
	m_onComboSequenceChanged.Broadcast();
}

int32 AInputComboManager::GetKeyComboIndex(FName key)
{
	if (key == "a")
	{
		return 0;
	}
	else if (key == "b")
	{
		return 1;
	}
	else if (key == "x")
	{
		return 2;
	}
	else if (key == "y")
	{
		return 3;
	}
	return -1;
}

FName AInputComboManager::GetComboIndexKey(int32 index)
{
	switch (index)
	{
	case 0:
		return "a";
	case 1:
		return "b";
	case 2:
		return "x";
	case 3:
		return "y";
	default:
		return "";
	}
}

bool AInputComboManager::ComboInputPressed(int32 playerIndex, int32 pressedKeyIndex)
{
	if (!m_playerComboProgress.Contains(playerIndex))
	{
		m_playerComboProgress.Add(playerIndex, 0);
	}
	int32 comboProgress = m_playerComboProgress[playerIndex];
	if (GetKeyComboIndex(m_comboSequence[comboProgress]) == pressedKeyIndex)
	{
		m_onComboInputSuccess.Broadcast(playerIndex, comboProgress);
		m_playerComboProgress[playerIndex] = comboProgress + 1;
		if (m_playerComboProgress[playerIndex] == m_comboSequence.Num())
		{
			m_onComboSequenceComplete.Broadcast(playerIndex);
			m_playerComboProgress[playerIndex] = 0;
		}
		return true;
	}
	else
	{
		m_onComboInputFail.Broadcast(playerIndex);
		m_playerComboProgress[playerIndex] = 0;
	}
	return false;
}

