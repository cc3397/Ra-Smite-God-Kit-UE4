// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseWork.h"
#include "Enemy.h"
#include "BaseAutoAttack.h"

#define COLLISION_ENEMY ECollisionChannel::ECC_EngineTraceChannel1
#define COLLISION_ALLY ECollisionChannel::ECC_EngineTraceChannel2
#define COLLISION_PLAYER ECollisionChannel::ECC_EngineTraceChannel3
// Sets default values
AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collider = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Collision Component"));
	//collisionComp->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);

	collider->BodyInstance.SetCollisionProfileName("Enemy");
	collider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);

	collider->SetCollisionObjectType(COLLISION_ENEMY);
	collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	collider->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);

	RootComponent = collider;

	enemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Mesh"));
	enemyMesh->AttachTo(RootComponent);

	health = 500.0f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

float AEnemy::TakeDamage(float damage, FDamageEvent const & damageEvent, AController * eventInstigator, AActor * damageCauser)
{
	health -= damage;

	GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("%f"), damage));
	GEngine->AddOnScreenDebugMessage(2, 1, FColor::White, FString::Printf(TEXT("%f"), health));
	if (health <= 0.0f)
	{
		Destroy();
	}


	return 0.0f;
}


