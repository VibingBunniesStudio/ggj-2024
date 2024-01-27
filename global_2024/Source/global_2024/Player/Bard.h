
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChanged, int32, playerIndex, int32, remainingHealth);

UCLASS()
class GLOBAL_2024_API ABard : public ACharacter
{
	GENERATED_BODY()

private:

	int m_hp;
	int m_playerId;

public:
	static FHealthChanged m_onHealthChanged;

public:
	
	ABard();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage();

	UFUNCTION()
	void Attack(int32 playerIndex);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Attack"))
	void Attack_Internal();

protected:
	
	virtual void BeginPlay() override;

};
