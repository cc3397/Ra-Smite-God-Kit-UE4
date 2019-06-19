// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseWork.h"
#include "BaseAutoAttack.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseGodClass.h"
#include "Enemy.h"

//define collision channels
#define COLLISION_ALLY ECollisionChannel::ECC_EngineTraceChannel2
#define COLLISION_PLAYER ECollisionChannel::ECC_EngineTraceChannel3
#define COLLISION_AUTOATTACK ECollisionChannel::ECC_EngineTraceChannel4
#define COLLISION_ABILITYALLY ECollisionChannel::ECC_EngineTraceChannel5
// Sets default values
ABaseAutoAttack::ABaseAutoAttack(const FObjectInitializer& objectInitializer)
	:Super(objectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup sphere as a collider, could be changed for each gods auto
	collider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	collider->InitSphereRadius(25.0f);
	collider->SetCollisionObjectType(COLLISION_AUTOATTACK);
	collider->BodyInstance.SetCollisionProfileName("Projectile");
	collider->SetNotifyRigidBodyCollision(true);
	collider->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);
	collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	collider->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_ABILITYALLY, ECollisionResponse::ECR_Ignore);
	collider->SetCollisionResponseToChannel(COLLISION_ALLY, ECollisionResponse::ECR_Ignore);
	
	
	//set collider as root
	RootComponent = collider;

	//setup mesh
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//mesh->bGenerateOverlapEvents = false;
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->AttachTo(RootComponent);

	//setup projectile movement
	projectileController = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileController"));
	projectileController->InitialSpeed = projectileSpeed;
	projectileController->ProjectileGravityScale = 0.0f;
	projectileController->bAutoActivate = true;
	
	InitialLifeSpan = 0.4f;

	autoAttackDamage = 10;
}

void ABaseAutoAttack::BeginPlay()
{
	Super::BeginPlay();
	collider->OnComponentHit.AddDynamic(this, &ABaseAutoAttack::OnHit); //bind hit function
}

void ABaseAutoAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseAutoAttack::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	AEnemy* hitActor = Cast<AEnemy>(OtherActor); //cast otheractor to enemy
	
	if (hitActor == nullptr) 
	{
		Destroy();
		return;
	}
	else
	{	//deal damage then destroy attack
		TSubclassOf<UDamageType> dmg;
		FHitResult hitInfo;
		UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, dmg);
		Destroy();
	}
	
}


