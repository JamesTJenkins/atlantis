// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "CharacterInterface.h"
#include "AtlantisDefines.h"
#include "ShootableInterface.h"

AAtlantisProjectile::AAtlantisProjectile() {
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AAtlantisProjectile::OnHit);		// set up a notification for when this component hits something blocking

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;

	damage = 0;	// This gets set by the weapon shooting it
}

void AAtlantisProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if(OtherActor && OtherComp && OtherActor != this) {
		if(ICharacterInterface* enemy = Cast<ICharacterInterface>(OtherActor)) {
			enemy->UpdateHealth(-damage);
		} else {
			if (OtherComp->IsSimulatingPhysics())
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}

		if(OtherComp->ComponentHasTag(SHOOTABLE_TAG)) {
			if(IShootableInterface* shootable = Cast<IShootableInterface>(OtherActor)) {
				shootable->OnHit(OtherComp);
			}
		}

		Destroy();
	}
}