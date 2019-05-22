// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* barrelToSet) {

	barrel = barrelToSet;

}

void UTankAimingComponent::AimAt(FVector hitLocation, float launchSpeed) {

	if (!barrel)
		return;

	FVector outLaunchVelocity(0);
	FVector startLoc = barrel->GetSocketLocation(FName("Projectile"));
	// Calculate the outLaunchVelocity
	bool haveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, outLaunchVelocity, startLoc, hitLocation, launchSpeed, ESuggestProjVelocityTraceOption::DoNotTrace);
	if (haveAimSolution) {
		// Turn it into a unit vector
		FVector aimDir = outLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(aimDir);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector aimDir) {

	// work out difference between current barrel rotation and aim direction
	FRotator barrelRotation = barrel->GetForwardVector().Rotation();
	FRotator aimAsRotator = aimDir.Rotation();
	FRotator deltaRotator = aimAsRotator - barrelRotation;
	//UE_LOG(LogTemp, Warning, TEXT("aimAsRotator: %s"), *aimAsRotator.ToString())

	barrel->Elevate(5); // TODO remove magic number
}

