#ifndef _GAME_CAVE_CUSTOMGEN_H
#define _GAME_CAVE_CUSTOMGEN_H

#include "types.h"
#include "Game/mapParts.h"
#include "Game/Cave/RandMapUnit.h"
#include "JSystem/JUtility/JUTTexture.h"

struct Graphics;

namespace Game {
namespace Cave {

	// Increment all of these for each new BaseGen. Also add your new BaseGens IN PROPER CONSECUTIVE ORDER to enemyTypes array.
	int mRealTypeCount[38];
	int mRealTypeMax[38];
	const int mMaxTypes = 38;
	int enemyTypes[38] = { 
		BaseGen::CGT_EnemyEasy, 
		BaseGen::CGT_EnemyHard, 
		BaseGen::CGT_DoorSeam, 
		BaseGen::CGT_EnemySpecial, 
		BaseGen::CGT_Easy2,
		BaseGen::CGT_Easy3,
		BaseGen::CGT_Easy4,
		BaseGen::CGT_Easy5,
		BaseGen::CGT_NewEnemy1,
		BaseGen::CGT_NewEnemy2,
		BaseGen::CGT_NewEnemy3,
		BaseGen::CGT_NewEnemy4,
		BaseGen::CGT_NewEnemy5,
		BaseGen::CGT_NewEnemy6,
		BaseGen::CGT_NewEnemy7,
		BaseGen::CGT_NewEnemy8,
		BaseGen::CGT_NewEnemy9,
		BaseGen::CGT_NewEnemy10,
		BaseGen::CGT_NewEnemy11,
		BaseGen::CGT_NewEnemy12,
		BaseGen::CGT_NewEnemy13,
		BaseGen::CGT_NewEnemy14,
		BaseGen::CGT_NewEnemy15,
		BaseGen::CGT_NewEnemy16,
		BaseGen::CGT_NewEnemy17,
		BaseGen::CGT_NewEnemy18,
		BaseGen::CGT_NewEnemy19,
		BaseGen::CGT_NewEnemy20,
		BaseGen::CGT_NewEnemy21,
		BaseGen::CGT_NewEnemy22,
		BaseGen::CGT_NewEnemy23,
		BaseGen::CGT_NewEnemy24,
		BaseGen::CGT_NewEnemy25,
		BaseGen::CGT_NewEnemy26,
		BaseGen::CGT_NewEnemy27,
		BaseGen::CGT_NewEnemy28,
		BaseGen::CGT_NewEnemy29,
		BaseGen::CGT_NewEnemy30,
	};

	//int randseed[20];
	//int randitemseed[12];

} // namespace Cave
} // namespace Game

#endif
