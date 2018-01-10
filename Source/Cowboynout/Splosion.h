// you no take candle!

#pragma once

#include "GameFramework/Actor.h"
#include "Splosion.generated.h"

UCLASS()
class COWBOYNOUT_API ASplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplosion();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	float nadeTimer;
	
	UPROPERTY()
	float nadeTimerFull;

	UPROPERTY()
	bool theEnd;

	UPROPERTY()
	bool doHit;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> hitEnemies;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* CollisionComp;

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void DoDmg(int damage, bool terminal);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
