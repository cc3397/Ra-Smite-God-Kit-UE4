// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseWork.h"
#include "Ability4_RA.h"
#include "Enemy.h"

//collision channels
#define COLLISION_ALLY ECollisionChannel::ECC_EngineTraceChannel2
#define COLLISION_PLAYER ECollisionChannel::ECC_EngineTraceChannel3
#define COLLISION_AUTOATTACK ECollisionChannel::ECC_EngineTraceChannel4
#define COLLISION_ABILITYALLY ECollisionChannel::ECC_EngineTraceChannel5

// Sets default values
AAbility4_RA::AAbility4_RA(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup collider
	collider = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Collision Component"));

	//setup collison responses to channels
	collider->SetCollisionObjectType(COLLISION_ABILITYALLY);
	collider->BodyInstance.SetCollisionProfileName("AbilityAlly");
	collider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly, true);
	collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	collider->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_ALLY, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_AUTOATTACK, ECollisionResponse::ECR_Ignore);
	collider->OnComponentBeginOverlap.AddDynamic(this, &AAbility4_RA::OnBeginOverLap);

	RootComponent = collider;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //tell mesh to ignore all collision, the collider will handle collisions.
	mesh->bGenerateOverlapEvents = false; //stop the mesh from firing events (like take damage)
	mesh->AttachTo(RootComponent);

	InitialLifeSpan = 0.2f; //to die instantly
}

// Called when the game starts or when spawned
void AAbility4_RA::BeginPlay()
{
	Super::BeginPlay();

	if (material != nullptr) //if there is a material defined, set it
	{
		mesh->SetMaterial(0, material);
	}
	
}

// Called every frame
void AAbility4_RA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbility4_RA::OnBeginOverLap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AEnemy* hitActor = Cast<AEnemy>(OtherActor); //cast other actor to enemy

	if (hitActor == nullptr) //if Hit actor is not an enemy then return and dont apply damage
	{
		return;
	}
	else
	{	//deal damage
		TSubclassOf<UDamageType> dmg;
		FHitResult hitInfo;
		UGameplayStatics::ApplyDamage(OtherActor, damageAmount, nullptr, this, dmg);
	}
}

