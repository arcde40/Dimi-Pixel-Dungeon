#pragma once

typedef struct Player {
	int Health, maxHealth;
	int playerX, playerY;
	int inventory[16];
	int equippedWeapon;
	int equippedArmor;
	int equippedAccesory[2];
}Player;