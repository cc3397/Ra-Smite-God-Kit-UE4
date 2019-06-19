// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseGodClass.generated.h"

UCLASS()
class COURSEWORK_API ABaseGodClass : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	ABaseGodClass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//setup function to take damage/healing
	virtual float TakeDamage(float damage, struct FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;

	//setup camera components
	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* springArm;

	//abilities / auto attacks to be spawned
	UPROPERTY(EditDefaultsOnly, Category = "AutoAttack")
		TSubclassOf<class ABaseAutoAttack> AutoAttackClass;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
		TSubclassOf<class AAbility_1RA> Ability1Class;

	
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
		TSubclassOf<class AAbility2_RA> Ability2Class;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
		TSubclassOf<class AAbility3_RA> Ability3Class;


	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
		TSubclassOf<class AAbility4_RA> Ability4Class;
	
	UWorld* World; //world for spawning abilities

	FVector2D mouseInput;

	FVector offset; //standard offset for ability spawning

	void MoveForward(float forwardMoveSpeed); //handle forward movement

	void MoveRight(float sideMoveSpeed); //handle side movements

	//seteup mouse movement controls
	void AddYaw(float yawVal); //handle mouse yaw

	void MousePitch(float axisPitch); //handle mouse pitch

private:
	//varibales for timers controlling the cooldowns of abilities
	FTimerHandle autoAttackTimer;
	bool canFireAuto;

	FTimerHandle abilityOneTimer;
	bool canFireAbilityOne;
	UPROPERTY(EditAnywhere, Category = "Abilities")
		float abilityOneCooldown;

	FTimerHandle abilityTwoTimer;
	bool canFireAbilityTwo;
	UPROPERTY(EditAnywhere, Category = "Abilities")
		float abilityTwoCooldown;

	FTimerHandle abilityThreeTimer;
	bool canFireAbilityThree;
	UPROPERTY(EditAnywhere, Category = "Abilities")
		float abilityThreeCooldown;

	FTimerHandle abilityFourTimer;
	bool canFireAbilityFour;
	UPROPERTY(EditAnywhere, Category = "Abilities")
		float abilityFourCooldown;

	FTimerHandle passiveAbilityTimer;
	float moveSpeedModifier;

protected:
	//functions to handle mouse presses and ability presses
	void OnAutoAttackFire();

	void OnAbility1Fire();

	void OnAbility2Fire();

	void OnAbility3Fire();

	void OnAbility4Fire();

	//functions to reset cooldowns of abilities and auto attacks
	UFUNCTION()
		void ResetAutoAttack();
	UFUNCTION()
		void ResetAbilityOne();
	UFUNCTION()
		void ResetAbilityTwo();
	UFUNCTION()
		void ResetAbilityThree();
	UFUNCTION()
		void ResetAbilityFour();

	UFUNCTION()
		void ResetPassiveMoveSpeed();
	
	//setup health of player
	UPROPERTY(EditAnywhere, Category = "Health")
		float health;


};
