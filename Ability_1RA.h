// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ability_1RA.generated.h"

UCLASS()
class COURSEWORK_API AAbility_1RA : public AActor
{
	GENERATED_BODY()
	
protected:	
	// Sets default values for this actor's properties
	AAbility_1RA(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//collision component
	UPROPERTY(EditAnywhere, Category = "Collision Component")
		UBoxComponent* collider;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterial* material;
	
	//positions for ability movement
	FVector currentPos;
	FVector targetPos;
	
	UFUNCTION() //handle collison
		void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Damage")
		float damageAmount;
	
};
