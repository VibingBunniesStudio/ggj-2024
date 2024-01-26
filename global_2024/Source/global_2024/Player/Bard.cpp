
#include "Bard.h"

ABard::ABard()
{
	PrimaryActorTick.bCanEverTick = true;
	m_hp = 3;
}

void ABard::BeginPlay()
{
	Super::BeginPlay();
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
