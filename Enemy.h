// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class COURSEWORK_API AEnemy : public AActor
{
	GENERATED_UCLASS_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		 UBoxComponent* collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* enemyMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//add ability to take damage
	virtual float TakeDamage(float damage, struct FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;

	UPROPERTY(EditAnywhere, Category = "Health")
		float health;

	
};
