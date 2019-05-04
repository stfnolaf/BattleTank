// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();

	auto controlledTank = GetControlledTank();
	if (!controlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not possessing a tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing: %s"), *(controlledTank->GetName()));
	}
}

ATank* ATankPlayerController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());

}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair() {

	if (!GetControlledTank())
		return;

	FVector hitLocation;
	if (GetSightRayHitLocation(hitLocation)) {
		GetControlledTank()->AimAt(hitLocation);
	}

}

bool ATankPlayerController::GetSightRayHitLocation(FVector& hitLocation) const {

	int32 viewportSizeX, viewportSizeY;
	GetViewportSize(viewportSizeX, viewportSizeY);
	FVector2D screenLocation = FVector2D(viewportSizeX * CrosshairXLocation, viewportSizeY * CrosshairYLocation);
	FVector lookDirection;
	if (GetLookDirection(screenLocation, lookDirection)) {
			// line trace along that direction and see what we hit
			GetLookVectorHitLocation(lookDirection, hitLocation);
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D screenLocation, FVector& worldDirection) const {

	FVector worldLocation;
	return DeprojectScreenPositionToWorld(screenLocation.X, screenLocation.Y, worldLocation, worldDirection);

}

bool ATankPlayerController::GetLookVectorHitLocation(FVector lookDirection, FVector& hitLocation) const {

	FHitResult hitResult;
	FVector startLoc = PlayerCameraManager->GetCameraLocation();
	FVector endLoc = startLoc + (lookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startLoc, endLoc, ECollisionChannel::ECC_Visibility)) {
		hitLocation = hitResult.Location;
		return true;
	}
	hitLocation = FVector(0.0f);
	return false;
}
