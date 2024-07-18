// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

class USphereComponent;
class URotatingMovementComponent;

UCLASS()
class MULTIPLAYERGAME_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* RotatingComponent;

	UPROPERTY(ReplicatedUsing = OnRep_CoinValue)
	uint8 CoinValue;

	UFUNCTION()
	void OnRep_CoinValue();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* BronzeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* SilverMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* GoldMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DestroySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* DestroyEffect;

	UFUNCTION(Server, Reliable)
	void InitCoin();

	UFUNCTION(NetMulticast, Reliable)
	void DestroyCoin();

	UFUNCTION()
	void OnBeginOverlapComponentEvent(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
