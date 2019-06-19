// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseWork.h"
#include "Ability_1RA.h"
#include "Enemy.h"

//collision channels
#define COLLISION_ALLY ECollisionChannel::ECC_EngineTraceChannel2
#define COLLISION_PLAYER ECollisionChannel::ECC_EngineTraceChannel3
#define COLLISION_AUTOATTACK ECollisionChannel::ECC_EngineTraceChannel4
#define COLLISION_ABILITYALLY ECollisionChannel::ECC_EngineTraceChannel5

// Sets default values
AAbility_1RA::AAbility_1RA(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup collider
	collider = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Collision Component"));

	//setup collision responses/channels
	collider->SetCollisionObjectType(COLLISION_ABILITYALLY);
	collider->BodyInstance.SetCollisionProfileName("AbilityAlly");
	collider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly, true);
	collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	collider->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_ALLY, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_AUTOATTACK, ECollisionResponse::ECR_Ignore);
	collider->OnComponentBeginOverlap.AddDynamic(this, &AAbility_1RA::OnBeginOverLap);
	
	RootComponent = collider;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); //tell mesh to ignore all collision, the collider will handle collisions.
	mesh->bGenerateOverlapEvents = false; //stop the mesh from firing events (like take damage)
	mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AAbility_1RA::BeginPlay()
{
	Super::BeginPlay();
	currentPos = GetActorLocation(); //get the current position
	targetPos = (GetActorLocation() + GetActorRotation().RotateVector(FVector(800.0f, 0.0f, 0.0f))); //get target position 800 units infront of player

	//set material
	if (material != nullptr)
	{
		mesh->SetMaterial(0, material);
	}
	
}

// Called every frame
void AAbility_1RA::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );	
	currentPos = GetActorLocation(); //get current position
	SetActorLocation(FMath::VInterpConstantTo(currentPos, targetPos, DeltaTime, 600.0f), true); //set the location to the interpolated point

	if (currentPos == targetPos) //destroy when target point is reached
	{
		Destroy();
	}


}

void AAbility_1RA::OnBeginOverLap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AEnemy* hitActor = Cast<AEnemy>(OtherActor); //cast otheractor to enemy class

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





