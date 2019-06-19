// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability3_RA.generated.h"

UCLASS()
class COURSEWORK_API AAbility3_RA : public AActor
{
	GENERATED_BODY()
	
protected:	
	// Sets default values for this actor's properties
	AAbility3_RA(const FObjectInitializer& ObjectInitializer);

	//collision component
	UPROPERTY(EditAnywhere, Category = "Collision Component")
		USphereComponent* collider;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterial* material;

	UWorld* World; //reference to game world

	//timer to handle tick rate
	FTimerHandle tickTimer;
	UPROPERTY(EditAnywhere, Category = "Damage")
		float damageAmount;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
		void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//variables for causing tick damage
	bool canTick;
	void PlayerTickStart();
};
