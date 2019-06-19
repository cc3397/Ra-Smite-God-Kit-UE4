// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseAutoAttack.generated.h"

UCLASS()
class COURSEWORK_API ABaseAutoAttack : public AActor
{
	GENERATED_BODY()

protected:
	ABaseAutoAttack(const FObjectInitializer& objectInitializer);// Sets default values for this actor's properties
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	//Sphere collision component 
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* collider;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, Category = "ProjectileController")
		UProjectileMovementComponent* projectileController;

	UPROPERTY(EditAnywhere, Category = "ProjectileController")
		float projectileSpeed = 1500.0f; //set as 2000 by default, can be changed in editor

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
	int autoAttackDamage; //damage dealt by attack

};
