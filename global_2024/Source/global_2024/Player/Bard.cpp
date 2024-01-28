
#include "Bard.h"
#include "../InputCombo/InputComboManager.h"
#include <Kismet/GameplayStatics.h>
#include "EnhancedInputSubsystems.h"

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
	// Get the player controller
	if (APlayerController* PC = GetController<APlayerController>())
	{
		const ULocalPlayer* localPlayer = PC->GetLocalPlayer();
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (Subsystem != nullptr)
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();
			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.0
			Subsystem->AddMappingContext(m_mappingContext, 0);
			//Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}
}

void ABard::TakeDamage()
{
	if (m_hp > 0) {
		TakeDamage_Internal();
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
