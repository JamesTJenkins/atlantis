// Copyright James Jenkins All Rights Reserved.

#include "Enemy/BaseEnemyCharacter.h"

ABaseEnemyCharacter::ABaseEnemyCharacter() : Super() {
	maxHealth = 100;
	health = 100;
	damage = 25;
}

void ABaseEnemyCharacter::UpdateHealth(float value) {
	health += value;

	if (health < 0) {
		// For enemy deaths we will just destroy them for now
		// will come back if need to do ragdoll or death animations
		Destroy();
	}
}
