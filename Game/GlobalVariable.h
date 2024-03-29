﻿#pragma once



// Map

#define MIN_ROOM_COUNT 4
#define MAX_ROOM_COUNT 10
#define MAX_ROOM_WEIGHT 15
#define MAX_ROOM_HEIGHT 15
#define MIN_ROOM_HEIGHT 8
#define MIN_ROOM_WEIGHT 8
#define MIN_X 20
#define MIN_Y 20
#define MAX_X 60
#define MAX_Y 60
#define CORRIDOR_GENERATE_TRIES 40
#define EXIT_GENERATE_TRIES 100
#define MAP_Y_SIZE MIN_Y+MAX_Y

#define MAP_X_START 4
#define MAP_X_END 45
#define MAP_Y_START 6
#define MAP_Y_END 128

#define MAX_FLOOR 1

// COLOR
#define COLOR_BLACK 30
#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_YELLOW 33
#define COLOR_BLUE 34
#define COLOR_MAGENTA 35
#define COLOR_CYAN 36
#define COLOR_WHITE 37

#define COLOR_CHECKED 98

#define COLOR_BRIGHT_BLACK 90
#define COLOR_BRIGHT_RED 91
#define COLOR_BRIGHT_GREEN 92
#define COLOR_BRIGHT_YELLOW 93
#define COLOR_BRIGHT_BLUE 94
#define COLOR_BRIGHT_MAGENTA 95
#define COLOR_BRIGHT_CYAN 96
#define COLOR_BRIGHT_WHITE 97

// Other

#define MAX_FRAME 10
#define MOVING_FRAME 10

// Render

#define POPUP_TOP 0
#define POPUP_CENTER 1
#define POPUP_BOTTOM 2
#define RENDERRANGE_X 20
#define RENDERRANGE_Y 60

// Mob AI
#define MOB_BEHAVE_STAY 0
#define MOB_BEHAVE_HOSTILE 1
#define MOB_BEHAVE_RANGED 2
#define MOB_BEHAVE_PASSIVE 3

#define MOB_DETECT_RANGE_SHORT 5
#define MOB_DETECT_RANGE_MEDIUM 10
#define MOB_DETECT_RANGE_LONG 15
#define MOB_DETECT_RANGE_INFINITE 100

#define MOB_MAX_COUNT 15
#define MOB_MIN_COUNT 4

// Item Type
#define ITEM_WEAPON 0x1
#define ITEM_ACCESORY 0x2
#define ITEM_ARMOR 0x3
#define ITEM_FOOD 0x4
#define ITEM_UTILITY 0x5
#define ITEM_POTION 0x6
#define ITEM_MISC 0x7

// Item
#define ITEM_STACKABLE 0x20
#define ITEM_POTIONALLOCATED 0x1F
#define ITEM_IDENTIFIABLE 0x40
#define ITEM_UPGRADABLE 0x80
#define ITEM_IDENTIFIED 0x100
#define ITEM_CONSUMABLE 0x200
#define NA -1
// Weapon

#define DEFAULT_ACCURACY 0.8

// Utility

#define	ABS(x)			( ((x)<0)?-(x):(x) )
#define UPDATESIG updateSignal = true;
#define CheckKeyPressed(x) (GetAsyncKeyState(x) && !keyInputState[x])

#define MaxExp(x) (x*x + 4*x + 10)