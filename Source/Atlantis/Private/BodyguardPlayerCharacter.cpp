// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyguardPlayerCharacter.h"
#include "AtlantisWeaponComponent.h"

ABodyguardPlayerCharacter::ABodyguardPlayerCharacter() : Super() {
	primary = CreateDefaultSubobject<UAtlantisWeaponComponent>(TEXT("primary"));
	primary->SetupAttachment(GetMesh1P(), FName(TEXT("GripPoint")));
	primary->Character = this;
}

void ABodyguardPlayerCharacter::Fire() {
	if (primary == nullptr)
		return;

	primary->Fire();
}
