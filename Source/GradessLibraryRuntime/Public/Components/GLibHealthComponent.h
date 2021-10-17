// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GLibHealthComponent.generated.h"

#pragma region LogDeclarations
DECLARE_LOG_CATEGORY_EXTERN(LogHealthComponent, Log, All);
#pragma endregion LogDeclarations

#pragma region DelegateDeclarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealth);
#pragma endregion DelegateDeclarations

/**
 * @brief Component that is responsible for health of the object. Track when someone damage owning actor.
 */
UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	),
	meta = (
		BlueprintSpawnableComponent
	)
)
class GRADESSLIBRARYRUNTIME_API UGLibHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGLibHealthComponent();

	virtual void BeginPlay() override;

protected:
	UFUNCTION(
		BlueprintNativeEvent,
		Category = "JoyWay|HealthComponent"
	)
	void OnTakeAnyDamage(
		AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy,
		AActor* DamageCauser
	);

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "JoyWay|HealthComponent"
	)
	void OnTakePointDamage(
		AActor* DamagedActor,
		float Damage,
		class AController* InstigatedBy,
		FVector HitLocation,
		class UPrimitiveComponent* FHitComponent,
		FName BoneName,
		FVector ShotFromDirection,
		const class UDamageType* DamageType,
		AActor* DamageCauser
	);

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "JoyWay|HealthComponent"
	)
	void OnTakeRadialDamage(
		AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		FVector Origin,
		FHitResult HitInfo,
		class AController* InstigatedBy,
		AActor* DamageCauser
	);

	/**
	* @brief Decrease health
	* @param Amount number for decreasing health
	*/
	UFUNCTION(
		BlueprintNativeEvent,
		BlueprintAuthorityOnly,
		Category = "JoyWay|HealthComponent"
	)
	void DecreaseHealth(
		float Amount
	);

	/**
	* @brief Returns current health value
	* @return current health
	*/
	UFUNCTION(
		BlueprintGetter,
		Category = "JoyWay|HealthComponent"
	)
	float GetHealth() const;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "JoyWay|HealthComponent"
	)
	void OnRep_Health();

public:
	UPROPERTY(
		BlueprintAssignable,
		Category = "JoyWay|HealthComponent"
	)
	FOutOfHealth OnOutOfHealth;

private:
	/**
	* @brief Current health
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintGetter = GetHealth,
		ReplicatedUsing = OnRep_Health,
		Category = "JoyWay|HealthComponent"
	)
	float Health;
};
