// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability4_RA.generated.h"

UCLASS()
class COURSEWORK_API AAbility4_RA : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility4_RA(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//collision component
	UPROPERTY(EditAnywhere, Category = "Collision Component")
		UBoxComponent* collider;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterial* material;

	UFUNCTION() //handle collision
		void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Damage")
		float damageAmount;
	
	
};
