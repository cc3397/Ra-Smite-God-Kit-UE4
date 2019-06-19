// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseWork.h"
#include "Ability3_RA.h"
#include "Enemy.h"
#include "BaseGodClass.h"

//collision channels
#define COLLISION_ALLY ECollisionChannel::ECC_EngineTraceChannel2
#define COLLISION_PLAYER ECollisionChannel::ECC_EngineTraceChannel3
#define COLLISION_AUTOATTACK ECollisionChannel::ECC_EngineTraceChannel4
#define COLLISION_ABILITYALLY ECollisionChannel::ECC_EngineTraceChannel5

// Sets default values
AAbility3_RA::AAbility3_RA(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup collider
	collider = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision Component"));

	//setup collision responses/channels
	collider->SetCollisionObjectType(COLLISION_ABILITYALLY);
	collider->BodyInstance.SetCollisionProfileName("AbilityAlly");
	collider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly, true);
	collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	collider->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_ALLY, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_AUTOATTACK, ECollisionResponse::ECR_Ignore);
	collider->OnComponentBeginOverlap.AddDynamic(this, &AAbility3_RA::OnBeginOverLap);

	RootComponent = collider;

	//setup mesh
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //tell mesh to ignore all collision, the collider will handle collisions.
	mesh->bGenerateOverlapEvents = false; //stop the mesh from firing events (like take damage)
	mesh->AttachTo(RootComponent);

	//setup lifespan
	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AAbility3_RA::BeginPlay()
{
	Super::BeginPlay();

	//point to World
	World = GetWorld();

	//setup tick damage/healing
	canTick = false;
	World->GetTimerManager().SetTimer(tickTimer, this, &AAbility3_RA::PlayerTickStart, 1.0f, false);
	
}

// Called every frame
void AAbility3_RA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//set material
	if (material != nullptr)
	{
		mesh->SetMaterial(0, material);
	}
}

//handle collisions
void AAbility3_RA::OnBeginOverLap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//check to see whether the otheractor is an enemy or a player
		if (OtherActor->IsA(AEnemy::StaticClass()))
		{
			AEnemy* hitActor = Cast<AEnemy>(OtherActor); //cast otheractor to enemy

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
		else if (OtherActor->IsA(ABaseGodClass::StaticClass()))
		{

			ABaseGodClass* hitActor = Cast<ABaseGodClass>(OtherActor); //cast otheractor to player

			if (hitActor == nullptr)
			{
				return;
			}
			else
			{	//heal player
				TSubclassOf<UDamageType> dmg;
				FHitResult hitInfo;
				UGameplayStatics::ApplyDamage(OtherActor, -damageAmount, nullptr, this, dmg);
			}
		}
}

void AAbility3_RA::PlayerTickStart()
{
	//the bool flips each run, creating a tick effect
	if (!canTick)
	{
		AActor::SetActorEnableCollision(true); //re-enable collision
		canTick = true;
		World->GetTimerManager().SetTimer(tickTimer, this, &AAbility3_RA::PlayerTickStart, 0.5f, false); //reset timer
	}
	else
	{
		AActor::SetActorEnableCollision(false); //disable collision
		canTick = false;
		World->GetTimerManager().SetTimer(tickTimer, this, &AAbility3_RA::PlayerTickStart, 0.5f, false); //reset timer
	}
}

