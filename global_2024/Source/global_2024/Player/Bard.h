
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChanged, int32, playerIndex, int32, remainingHealth);

class UInputMappingContext;
UCLASS()
class GLOBAL_2024_API ABard : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	int m_hp;
	UPROPERTY(BlueprintReadOnly)
	int m_playerId;

	static FHealthChanged m_onHealthChanged;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* m_mappingContext;

public:
	
	ABard();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Take Damage"))
	void TakeDamage_Internal();

	UFUNCTION()
	void Attack(int32 playerIndex);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Attack"))
	void Attack_Internal();

protected:
	
	virtual void BeginPlay() override;

};
