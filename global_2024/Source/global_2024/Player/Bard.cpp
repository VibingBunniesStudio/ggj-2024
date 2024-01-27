
#include "Bard.h"
#include "../InputCombo/InputComboManager.h"
#include <Kismet/GameplayStatics.h>

ABard::ABard()
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
}

void ABard::TakeDamage()
{
	if (m_hp > 0) {
		m_hp--;
	}
}

void ABard::Attack(int32 playerIndex)
{
	int32 playerId = UGameplayStatics::GetPlayerControllerID(Cast<APlayerController>(GetController()));
	if (playerId == playerIndex)
	{
		Attack_Internal();
	}
}
