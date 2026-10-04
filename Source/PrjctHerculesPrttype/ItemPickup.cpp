#include "ItemPickup.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

AItemPickup::AItemPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(SceneRoot);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(SceneRoot);
	InteractionSphere->InitSphereRadius(150.0f);

	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetGenerateOverlapEvents(true);

	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(
		this,
		&AItemPickup::OnInteractionBegin);

	InteractionSphere->OnComponentEndOverlap.AddDynamic(
		this,
		&AItemPickup::OnInteractionEnd);

	CurrentPlayer = nullptr;
	bPlayerInRange = false;
}

void AItemPickup::OnInteractionBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);

	if (!Character || !Character->IsPlayerControlled())
	{
		return;
	}

	bPlayerInRange = true;
	CurrentPlayer = Character;

	UE_LOG(
		LogTemp,
		Log,
		TEXT("Player entered pickup range: %s"),
		*GetName());
}

void AItemPickup::OnInteractionEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor != CurrentPlayer)
	{
		return;
	}

	bPlayerInRange = false;
	CurrentPlayer = nullptr;

	UE_LOG(
		LogTemp,
		Log,
		TEXT("Player left pickup range: %s"),
		*GetName());
}