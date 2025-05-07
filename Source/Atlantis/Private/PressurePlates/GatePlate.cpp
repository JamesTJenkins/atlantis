// Copyright James Jenkins All Rights Reserved.

#include "PressurePlates/GatePlate.h"

AGatePlate::AGatePlate() {
	PrimaryActorTick.bCanEverTick = true;
}

void AGatePlate::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	FVector loc = gate->GetActorLocation();
	if(pressed > 0) {
		if(loc.Z >= highestZPos)
			return;
		UE_LOG(LogTemp, Log, TEXT("raising"));
		loc.Z += raiseAmount * deltaTime;

		if(loc.Z > highestZPos)
			loc.Z = highestZPos;

		gate->SetActorLocation(loc);
	} else {
		if(!IsRaised())
			return;
		UE_LOG(LogTemp, Log, TEXT("lowering"));
		loc.Z -= lowerAmount * deltaTime;

		if(loc.Z < lowestZPos)
			loc.Z = lowestZPos;

		gate->SetActorLocation(loc);
	}
}

bool AGatePlate::IsRaised() {
	return gate->GetActorLocation().Z > lowestZPos;
}

