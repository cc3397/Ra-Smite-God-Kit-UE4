// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseWork.h"
#include "BaseGodClass.h"
#include "Classes/Camera/CameraComponent.h"
#include "BaseAutoAttack.h"
#include "Ability_1RA.h"
#include "Ability2_RA.h"
#include "Ability3_RA.h"
#include "Ability4_RA.h"

#define COLLISION_ENEMY ECollisionChannel::ECC_EngineTraceChannel1

// Sets default values
ABaseGodClass::ABaseGodClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//setup collision
	GetCapsuleComponent()->SetCollisionObjectType(COLLISION_PLAYER);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Player");
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Block); //allows for "body blocking" in which enemies can prevent you from running away by standing infront of you

	
	RootComponent = GetCapsuleComponent();
	//Setup Camera
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->AttachTo(RootComponent);
	springArm->TargetArmLength = 350.0f;
	springArm->SetWorldRotation(FRotator(-60.0f, 0.0f, 0.0f));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(springArm, USpringArmComponent::SocketName);

	//auto possess the god on screen
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	offset = FVector(20.0f, 0.0f, 40.0f); //default offset for ability spawns

	//health component
	health = 1000.0f;
}

// Called when the game starts or when spawned
void ABaseGodClass::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld(); //get the game world

	//set up all bools for cooldowns to be true on begin play
	canFireAuto = true;
	canFireAbilityOne = true;
	canFireAbilityTwo = true;
	canFireAbilityThree = true;
	canFireAbilityFour = true;

	//set pasive move speed modifier to be 1 (will return same value as initial move speed after multiplication)
	moveSpeedModifier = 1.0f;
	
}

// Called every frame
void ABaseGodClass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//handle pitch of spring arm
	FRotator newPitch = springArm->GetComponentRotation();
	newPitch.Pitch = FMath::Clamp(newPitch.Pitch + mouseInput.Y, -80.0f, 0.0f);
	springArm->SetWorldRotation(newPitch);
	
	
}

// Called to bind functionality to input
void ABaseGodClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//bind movement settings
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseGodClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseGodClass::MoveRight);

	//bind mouse
	PlayerInputComponent->BindAxis("MouseYaw", this, &ABaseGodClass::AddYaw);
	PlayerInputComponent->BindAxis("MousePitch", this, &ABaseGodClass::MousePitch);
	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &ABaseGodClass::OnAutoAttackFire);

	//bind ability uses
	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &ABaseGodClass::OnAbility1Fire);
	PlayerInputComponent->BindAction("Ability2", IE_Pressed, this, &ABaseGodClass::OnAbility2Fire);
	PlayerInputComponent->BindAction("Ability3", IE_Pressed, this, &ABaseGodClass::OnAbility3Fire);
	PlayerInputComponent->BindAction("Ability4", IE_Pressed, this, &ABaseGodClass::OnAbility4Fire);
}

//apply damage/healing to player
float ABaseGodClass::TakeDamage(float damage, FDamageEvent const & damageEvent, AController * eventInstigator, AActor * damageCauser)
{
	health -= damage;
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("%f"), damage));
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("%f"), health));

	if (health <= 0) //if health goes below 0 destroy player
	{
		Destroy();
	}

	return 0.0f;
}

void ABaseGodClass::MoveForward(float forwardMoveSpeed) //Move God Forward
{
	if (Controller && forwardMoveSpeed)
	{
		AddMovementInput(GetActorForwardVector(), (forwardMoveSpeed* moveSpeedModifier));
	}
}

void ABaseGodClass::MoveRight(float sideMoveSpeed) //Move God Sideways
{
	if (Controller && sideMoveSpeed)
	{
		AddMovementInput(GetActorRightVector(), (sideMoveSpeed * moveSpeedModifier));
	}
}

void ABaseGodClass::AddYaw(float yawVal) //handle the players rotation via the mouse
{
	if (yawVal != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(yawVal);
	}
}

void ABaseGodClass::MousePitch(float axisPitch) //handle moving camera up and down via the mouse
{
	mouseInput.Y = axisPitch;
}

void ABaseGodClass::OnAutoAttackFire()
{
	if (canFireAuto) //check to see whether the timer for auto attack has ended
	{
		if (AutoAttackClass != NULL)
		{

			if (World != NULL)
			{
				const FRotator spawnRotation = GetControlRotation();

				const FVector spawnLocation = (GetActorLocation() + spawnRotation.RotateVector(offset)); //Offset the spawn location so Auto Attack spawns infront of player

				FActorSpawnParameters actorSpawnParams;
				//actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				World->SpawnActor<ABaseAutoAttack>(AutoAttackClass, spawnLocation, spawnRotation, actorSpawnParams); //Spawn The Auto Attack

				canFireAuto = false;
				World->GetTimerManager().SetTimer(autoAttackTimer, this, &ABaseGodClass::ResetAutoAttack, 0.5f, false);
			}
		}
	}
}

void ABaseGodClass::OnAbility1Fire()
{
	if (canFireAbilityOne)
	{
		if (Ability1Class != NULL)
		{

			if (World != NULL)
			{
				const FRotator spawnRotation = GetControlRotation();

				const FVector spawnLocation = (GetActorLocation() + spawnRotation.RotateVector(offset));//offset ability spawn

				FActorSpawnParameters actorSpawnParams;

				World->SpawnActor<AAbility_1RA>(Ability1Class, spawnLocation, spawnRotation, actorSpawnParams);//spawn ability

				//set cooldowns
				canFireAbilityOne = false;
				World->GetTimerManager().SetTimer(abilityOneTimer, this, &ABaseGodClass::ResetAbilityOne, abilityOneCooldown, false); //cooldown set in editor

			//handle passive ability
				if (moveSpeedModifier <= 2.0f) //cap the movement speed modification at 100% increase
				{
					moveSpeedModifier += 0.1f;
				}
				if (World->GetTimerManager().IsTimerActive(passiveAbilityTimer))
				{
					//if the timer is already active, then clear it first, then reset it
					World->GetTimerManager().ClearTimer(passiveAbilityTimer);
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}
				else
				{
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}
			}
		}
	}

}

void ABaseGodClass::OnAbility2Fire()
{
	if (canFireAbilityTwo)
	{
		if (Ability2Class != NULL)
		{

			if (World != NULL)
			{
				const FRotator spawnRotation = GetControlRotation();

				const FVector spawnLocation = (GetActorLocation() + spawnRotation.RotateVector(FVector(25.0f, -10.0f, 40.0f)));//offset ability spawn

				FActorSpawnParameters actorSpawnParams;

				World->SpawnActor<AAbility2_RA>(Ability2Class, spawnLocation, spawnRotation, actorSpawnParams);//spawn ability

				//set cooldown
				canFireAbilityTwo = false;
				World->GetTimerManager().SetTimer(abilityTwoTimer, this, &ABaseGodClass::ResetAbilityTwo, abilityTwoCooldown, false); //cooldown set in editor

			 //handle passive ability
				if (moveSpeedModifier <= 2.0f) //cap the movement speed modification at 100%
				{
					moveSpeedModifier += 0.1f;
				}
				if (World->GetTimerManager().IsTimerActive(passiveAbilityTimer))
				{
					//if the timer is already active, then clear it first, then reset it
					World->GetTimerManager().ClearTimer(passiveAbilityTimer);
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}
				else
				{
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}
				

			}
		}
	}
}

void ABaseGodClass::OnAbility3Fire()
{
	if (canFireAbilityThree)
	{
		if (Ability3Class != NULL)
		{

			if (World != NULL)
			{
				const FRotator spawnRotation = GetControlRotation();

				const FVector spawnLocation = (GetActorLocation() + spawnRotation.RotateVector(FVector(525.0f, -10.0f, 40.0f)));//offset ability spawn

				FActorSpawnParameters actorSpawnParams;

				World->SpawnActor<AAbility3_RA>(Ability3Class, spawnLocation, spawnRotation, actorSpawnParams);//spawn ability

				//set cooldown
				canFireAbilityThree = false;
				World->GetTimerManager().SetTimer(abilityThreeTimer, this, &ABaseGodClass::ResetAbilityThree, abilityThreeCooldown, false); //cooldown set in editor

			//handle passive ability
				if (moveSpeedModifier <= 2.0f) //cap the movement speed modification at 100%
				{
					moveSpeedModifier += 0.1f;
				}
				if (World->GetTimerManager().IsTimerActive(passiveAbilityTimer))
				{
					//if the timer is already active, then clear it first, then reset it
					World->GetTimerManager().ClearTimer(passiveAbilityTimer);
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}
				else
				{
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}

			}
		}

	}
}

void ABaseGodClass::OnAbility4Fire()
{
	if (canFireAbilityFour)
	{

		if (Ability4Class != NULL)
		{

			if (World != NULL)
			{
				const FRotator spawnRotation = GetControlRotation();

				const FVector spawnLocation = (GetActorLocation() + spawnRotation.RotateVector(FVector(500.0f, 0.0f, 40.0f))); //offset ability spawn

				FActorSpawnParameters actorSpawnParams;

				World->SpawnActor<AAbility4_RA>(Ability4Class, spawnLocation, spawnRotation, actorSpawnParams); //spawn ability

				//set cooldown
				canFireAbilityFour = false;
				World->GetTimerManager().SetTimer(abilityFourTimer, this, &ABaseGodClass::ResetAbilityFour, abilityFourCooldown, false); //cooldown set in editor

			//handle passive ability
				if (moveSpeedModifier <= 2.0f) //cap the movement speed modification at 100%
				{
					moveSpeedModifier += 0.1f;
				}
				if (World->GetTimerManager().IsTimerActive(passiveAbilityTimer))
				{
					//if the timer is already active, then clear it first, then reset it
					World->GetTimerManager().ClearTimer(passiveAbilityTimer);
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}
				else
				{
					World->GetTimerManager().SetTimer(passiveAbilityTimer, this, &ABaseGodClass::ResetPassiveMoveSpeed, 10.0f, false);
				}

			}
		}
	}
}

//functions for cooldowns
void ABaseGodClass::ResetAutoAttack()
{
	canFireAuto = true;
}

void ABaseGodClass::ResetAbilityOne()
{
	canFireAbilityOne = true;
}

void ABaseGodClass::ResetAbilityTwo()
{
	canFireAbilityTwo = true;
}

void ABaseGodClass::ResetAbilityThree()
{
	canFireAbilityThree = true;
}

void ABaseGodClass::ResetAbilityFour()
{
	canFireAbilityFour = true;
}

void ABaseGodClass::ResetPassiveMoveSpeed()
{
	moveSpeedModifier = 1.0f; //reset modifier after 10 seconds of no ability cast
}

