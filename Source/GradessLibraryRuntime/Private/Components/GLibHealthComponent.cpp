// Copyright Gradess. All Rights Reserved.

#include "Components/GLibHealthComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

#define LOG(Message, ...) UE_LOG(LogHealthComponent, Log, TEXT(Message), ##__VA_ARGS__)
#define SERVER_LOG(Message) LOG("Server: %s", Message);
#define CLIENT_LOG(Message) LOG("Client: %s", Message);

DEFINE_LOG_CATEGORY(LogHealthComponent);

UGLibHealthComponent::UGLibHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	SetIsReplicatedByDefault(true);

	Health = 100.f;
}

void UGLibHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	LOG("BeginPlay")

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UGLibHealthComponent::OnTakeAnyDamage);
	GetOwner()->OnTakePointDamage.AddDynamic(this, &UGLibHealthComponent::OnTakePointDamage);
	GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UGLibHealthComponent::OnTakeRadialDamage);
}

void UGLibHealthComponent::OnTakeAnyDamage_Implementation(
	AActor* DamagedActor,
	const float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser
)
{
	SERVER_LOG("OnTakeAnyDamage");
	DecreaseHealth(Damage);
}

void UGLibHealthComponent::OnTakePointDamage_Implementation(
	AActor* DamagedActor,
	const float Damage,
	AController* InstigatedBy,
	FVector HitLocation,
	UPrimitiveComponent* FHitComponent,
	FName BoneName,
	FVector ShotFromDirection,
	const UDamageType* DamageType,
	AActor* DamageCauser
)
{
	SERVER_LOG("OnTakePointDamage");
	DecreaseHealth(Damage);
}

void UGLibHealthComponent::OnTakeRadialDamage_Implementation(
	AActor* DamagedActor,
	const float Damage,
	const UDamageType* DamageType,
	FVector Origin,
	FHitResult HitInfo,
	AController* InstigatedBy,
	AActor* DamageCauser
)
{
	SERVER_LOG("OnTakeRadialDamage");
	DecreaseHealth(Damage);
}

void UGLibHealthComponent::DecreaseHealth_Implementation(const float Amount)
{
	Health -= Amount;

	LOG("Server: Damage: %f; Health: %f", Amount, Health);
	
	if (!FMath::IsNearlyZero(Health, 0.1f)) { return; }

	OnOutOfHealth.Broadcast();
}

float UGLibHealthComponent::GetHealth() const
{
	return Health;
}

void UGLibHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGLibHealthComponent, Health);
}

void UGLibHealthComponent::OnRep_Health_Implementation()
{
	LOG("Client: %s => Health is: %f", *GetOwner()->GetName(), Health);
}
