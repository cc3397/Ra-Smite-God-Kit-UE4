// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ability2_RA.generated.h"

UCLASS()
class COURSEWORK_API AAbility2_RA : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility2_RA(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//collision component
	UPROPERTY(EditAnywhere, Category = "Collision Component")
		USphereComponent* collider;


	UPROPERTY(EditAnywhere, Category = "Light")
		UPointLightComponent* pointLight;

	UFUNCTION()
		void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UWorld* World;

	float lightIntensity;

	bool lightExplode;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float damageAmount;

	FTimerHandle fireTimer; //timer to handle when ability fires

	void SetFireAbility(); //sets the fireAbility bool to be true(for use in timer handle)
};
