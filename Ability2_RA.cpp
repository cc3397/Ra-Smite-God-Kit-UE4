// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseWork.h"
#include "Ability2_RA.h"
#include "Enemy.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"

//collision channels
#define COLLISION_ALLY ECollisionChannel::ECC_EngineTraceChannel2
#define COLLISION_PLAYER ECollisionChannel::ECC_EngineTraceChannel3
#define COLLISION_AUTOATTACK ECollisionChannel::ECC_EngineTraceChannel4
#define COLLISION_ABILITYALLY ECollisionChannel::ECC_EngineTraceChannel5


// Sets default values
AAbility2_RA::AAbility2_RA(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
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
	
	RootComponent = collider;

	//setup light
	lightIntensity = 100.0f;
	pointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	pointLight->bVisible = true;
	pointLight->Intensity = lightIntensity;
	pointLight->AttachTo(RootComponent);

	lightExplode = false; //light has not exploded yet

	InitialLifeSpan = 1.5f;

}

// Called when the game starts or when spawned
void AAbility2_RA::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();//get reference to game world

	collider->OnComponentBeginOverlap.AddDynamic(this, &AAbility2_RA::OnBeginOverLap); //bind overlap function

	World->GetTimerManager().SetTimer(fireTimer, this, &AAbility2_RA::SetFireAbility, 1.4f, false); //set the timer to begin

	AActor::SetActorEnableCollision(false); //disable collision until timer re-activates it

}

// Called every frame
void AAbility2_RA::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!lightExplode) //if light hasnt exploded yet add to its intensity each frame
	{
		lightIntensity += 300.0f;
	}
	pointLight->SetIntensity(lightIntensity);
	

	SetActorLocation(World->GetFirstPlayerController()->GetPawn()->GetActorLocation()); //make the ability follow the player

}

void AAbility2_RA::OnBeginOverLap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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

void AAbility2_RA::SetFireAbility()
{
	AActor::SetActorEnableCollision(true); //re enable collisions
	lightIntensity = 100000.0f; //to imitate light 'explosion'
	pointLight->SetIntensity(lightIntensity);
	lightExplode = true; //so no more intensity is added in tick function
}

