
#include "Bard.h"
#include "../InputCombo/InputComboManager.h"
#include <Kismet/GameplayStatics.h>

FHealthChanged ABard::m_onHealthChanged;

ABard::ABard() : m_playerId(-1)
{
	PrimaryActorTick.bCanEverTick = true;
	m_hp = 3;
}

void ABard::BeginPlay()
{
	Super::BeginPlay();
	AInputComboManager* comboManager = Cast<AInputComboManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AInputComboManager::StaticClass()));
	if (comboManager)
	{
		comboManager->m_onComboSequenceComplete.AddDynamic(this, &ABard::Attack);
	}

}

void ABard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	m_playerId = UGameplayStatics::GetPlayerControllerID(Cast<APlayerController>(GetController()));
}

void ABard::TakeDamage()
{
	if (m_hp > 0) {
		m_hp--;
		m_onHealthChanged.Broadcast(m_playerId, m_hp);
	}
}

void ABard::Attack(int32 playerIndex)
{
	if (m_playerId == playerIndex)
	{
		Attack_Internal();
	}
}
