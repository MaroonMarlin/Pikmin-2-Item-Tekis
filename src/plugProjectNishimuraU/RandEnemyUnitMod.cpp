#include "Game/Cave/RandMapMgr.h"
#include "Game/Cave/CustomGen.h"
#include "Dolphin/rand.h"

namespace Game {
namespace Cave {

void RandEnemyUnit::setEnemySlot()
{
	if (mTotalCount < mMaxEnemies) {
        setEnemyTypeA();
		setEnemyTypeB();
		setEnemyTypeF();
        setEnemyTypeC();
		for (int i = 4; i < mMaxTypes; i++)
		{
			if (i < 8) {
				setEnemyTypeJ(i);
			}
			setEnemyTypeI(i);
		}
	}
}

void RandEnemyUnit::setEnemyTypeWeight()
{
	int weightList[mMaxTypes]; // _28
	int countList[mMaxTypes];  // _18
	int totalWeights = 0;

	for (int i = 0; i < mMaxTypes; i++) {
		mRealTypeCount[i] = 0;
		mRealTypeMax[i]   = 0;
	}

	for (int i = 0; i < mMaxTypes; i++) {
		weightList[i] = 0;
		countList[i]  = 0;
	}

	FOREACH_NODE(EnemyNode, mGenerator->mMainEnemies->mChild, currEnemy)
	{
		TekiInfo* tekiInfo = currEnemy->getTekiInfo();
		if (tekiInfo) {
			for (int i = 0; i < mMaxTypes; i++) {
				if (tekiInfo->mType == enemyTypes[i]) {
					int weight = tekiInfo->mWeight % 10;
					int num    = tekiInfo->mWeight / 10;
					if (weight) {
						totalWeights += weight;
						weightList[i] += weight;
					}
					if (num) {
						mRealTypeMax[i] += num;
						countList[i] += num;
					}
				}
			}
		}
	}

	int tallyWeights[mMaxTypes];

	tallyWeights[0] = weightList[0];

	for (int i = 1; i < mMaxTypes; i++)
	{
		tallyWeights[i] = tallyWeights[i-1] + weightList[i];
	}

	int totalNum = countList[0];
	for (int i = 1; i < mMaxTypes; i++) {
		totalNum += countList[i];
	}

	for (totalNum; totalNum < mMaxEnemies; totalNum++) {
		int randEnemy = randInt(totalWeights);
		for (int i = 0; i < mMaxTypes; i++) {
			if (randEnemy < tallyWeights[i]) {
				mRealTypeMax[i]++;
				break;
			}
		}
	}
}

void RandEnemyUnit::setEnemyTypeC()
{
	if (mGenerator->mIsVersusMode) {
		setVersusHibaTypeC();
	}

	if (mRealTypeCount[TEKITYPE_C] < mRealTypeMax[TEKITYPE_C]) {
		if (mGenerator->mIsVersusMode) {
			setVersusEnemyTypeC();
		}
		if (mRealTypeCount[TEKITYPE_C] < mRealTypeMax[TEKITYPE_C]) {
			for (int i = 0; i < 100; i++) {
				int slot = -1;
				setSlotEnemyTypeC(slot, -1);
				setUnitRandEnemyTypeC();
				if (mMapTile && slot >= 0 && mEnemyUnit) {
					makeSetEnemyTypeC(mMapTile, slot, mEnemyUnit);
					if (mRealTypeCount[TEKITYPE_C] < mRealTypeMax[TEKITYPE_C]) {
						continue;
					}
					return;
				}
				return;
			}
		}
	}
}

void RandEnemyUnit::setVersusHibaTypeC()
{
	if (mIsVersusHiba) {
		TekiInfo* info = new TekiInfo;
		info->mEnemyID = EnemyTypeID::EnemyID_ElecHiba;
		info->mType    = BaseGen::CGT_DoorSeam;

		EnemyUnit* unit = new EnemyUnit;
		unit->mTekiInfo = info;

		FOREACH_NODE(MapNode, mGenerator->getPlacedNodes()->mChild, currTile)
		{
			if (currTile->mUnitInfo->getUnitKind() == UNITKIND_Room) {
				int numDoors = currTile->getNumDoors();
				for (int i = 0; i < numDoors; i++) {
					if (!currTile->isGateSetDoor(i)) {
						Vector3f pos     = currTile->getDoorGlobalPosition(i);
						f32 dir          = currTile->getDoorGlobalDirection(i);
						EnemyNode* enemy = new EnemyNode(unit, nullptr, 1);
						enemy->setGlobalData(pos, dir);
						enemy->setBirthDoorIndex(i);
						currTile->mEnemyNode->add(enemy);
					}
				}
			}
		}
	}
}

void RandEnemyUnit::setVersusEnemyTypeC()
{
	int count = 0;
	FOREACH_NODE(EnemyNode, mGenerator->mMainEnemies->mChild, currEnemy)
	{
		TekiInfo* info = currEnemy->getTekiInfo();
		if (info && info->mType == BaseGen::CGT_DoorSeam) {
			count += info->mWeight / 10;
			if (count > mRealTypeCount[TEKITYPE_C]) {
				int altNum     = (count - mRealTypeCount[TEKITYPE_C]) % 2;
				int roundedMax = ((count - mRealTypeCount[TEKITYPE_C]) / 2) * 2;

				int randIdx = randInt(2);
				for (int i = 0; i < roundedMax; i++, randIdx ^= 1) {
					int slot = -1;
					setSlotEnemyTypeC(slot, randIdx);

					if (mMapTile && slot >= 0 && mEnemyUnit) {
						makeSetEnemyTypeC(mMapTile, slot, mEnemyUnit);
						continue;
					}
					return;
				}

				if (altNum) {
					int slot = -1;
					setSlotEnemyTypeC(slot, -1);

					if (mMapTile && slot >= 0 && mEnemyUnit) {
						makeSetEnemyTypeC(mMapTile, slot, mEnemyUnit);
						continue;
					}
					return;
				}
			}
		}
	}
}

void RandEnemyUnit::setSlotEnemyTypeC(int& doorIdx, int vsColor)
{

	MapNode* mapTiles[256];   // 0x808
	int openDoorIndices[256]; // 0x408
	int doorScores[256];      // 0x8

	int counter    = 0; // r22
	int scoreTally = 0; // r21

	MapNode* placedNodes = mGenerator->getPlacedNodes(); // r17

	if (mGenerator->mIsVersusMode) { // Versus mode
		int vsScore = 0;
		int sign    = 0;
		if (vsColor == Blue) {
			MapNode* redOnyon = mMapScore->getFixObjNode(FIXNODE_VsRedOnyon);
			if (redOnyon) {
				vsScore = redOnyon->getVersusScore();
				sign    = -1;
			}
		} else if (vsColor == Red) {
			MapNode* blueOnyon = mMapScore->getFixObjNode(FIXNODE_VsBlueOnyon);
			if (blueOnyon) {
				vsScore = blueOnyon->getVersusScore();
				sign    = 1;
			}
		}
		MapNode* node;
		for (node = static_cast<MapNode*>(placedNodes->mChild); node; node = static_cast<MapNode*>(node->mNext)) {
			// caps are always connected to not-caps, so don't need to worry about them
			if (node->mUnitInfo->getUnitKind() == UNITKIND_Room || node->mUnitInfo->getUnitKind() == UNITKIND_Corridor) {
				int numDoors = node->getNumDoors();
				for (int i = 0; i < numDoors; i++) {
					if (!node->isGateSetDoor(i)) {
						mapTiles[counter]        = node;
						openDoorIndices[counter] = i;
						doorScores[counter]      = sign * (vsScore + mapTiles[counter]->getVersusScore());
						if (doorScores[counter] <= 0) {
							doorScores[counter] = 1;
						}
						scoreTally += doorScores[counter];
						counter++;
					}
				}
			}
		}

	} else { // story mode
		MapNode* node;
		for (node = static_cast<MapNode*>(placedNodes->mChild); node; node = static_cast<MapNode*>(node->mNext)) {
			// caps are always connected to not-caps, so don't need to worry about them
			if (node->mUnitInfo->getUnitKind() == UNITKIND_Room || node->mUnitInfo->getUnitKind() == UNITKIND_Corridor) {
				int numDoors = node->getNumDoors();

				int scoreWeight = 1; // corridor weight
				if (node->mUnitInfo->getUnitKind() == UNITKIND_Room) {
					scoreWeight = 100; // room weight
				}

				// make list of all open doors, their map tiles, and their score
				for (int i = 0; i < numDoors; i++) {
					if (!node->isGateSetDoor(i)) { // if door is open
						mapTiles[counter]        = node;
						openDoorIndices[counter] = i;
						doorScores[counter]      = scoreWeight;
						scoreTally += doorScores[counter];
						counter++;
					}
				}
			}
		}
	}

	mMapTile = nullptr;
	mSpawn   = nullptr;

	if (counter == 0) {
		return;
	}

	int randScoreThreshold = (f32)scoreTally * randFloat();
	int scoreCounter       = 0;
	for (int i = 0; i < counter; i++) {
		scoreCounter += doorScores[i];
		if (scoreCounter > randScoreThreshold) {
			mMapTile = mapTiles[i];
			doorIdx  = openDoorIndices[i];
			return;
		}
	}
}

void RandEnemyUnit::setUnitRandEnemyTypeC()
{
	EnemyUnit* enemyList[128];
	int weightList[128];

	int counter     = 0;
	int weightTally = 0;

	EnemyUnit* enemy;
	int weightCounter = 0;
	EnemyNode* node;

	for (node = static_cast<EnemyNode*>(mGenerator->mMainEnemies->mChild); node; node = static_cast<EnemyNode*>(node->mNext)) {
		enemy = node->mEnemyUnit;
		if (enemy->mTekiInfo && enemy->mTekiInfo->mType == BaseGen::CGT_DoorSeam) {
			int ones = enemy->mTekiInfo->mWeight % 10;
			int tens = enemy->mTekiInfo->mWeight / 10;
			if (tens) {
				weightCounter += tens;
				if (weightCounter > mRealTypeCount[TEKITYPE_C]) {
					mEnemyUnit = enemy;
					return;
				}
			}

			if (ones) {
				enemyList[counter]  = enemy;
				weightList[counter] = ones;
				weightTally += weightList[counter];
				counter++;
			}
		}
	}

	mEnemyUnit = nullptr;
	if (weightTally == 0) {
		return;
	}

	int randWeightThreshold = (f32)weightTally * randFloat();
	int ctr                 = 0;
	for (int i = 0; i < counter; i++) {
		ctr += weightList[i];
		if (ctr > randWeightThreshold) {
			mEnemyUnit = enemyList[i];
			return;
		}
	}
}

void RandEnemyUnit::makeSetEnemyTypeC(MapNode* tile, int doorIdx, EnemyUnit* enemy)
{
	Vector3f doorPos     = tile->getDoorGlobalPosition(doorIdx);
	f32 doorDir          = tile->getDoorGlobalDirection(doorIdx);
	EnemyNode* enemyNode = new EnemyNode(enemy, nullptr, 1);
	enemyNode->setGlobalData(doorPos, doorDir);
	enemyNode->setBirthDoorIndex(doorIdx);
	tile->mEnemyNode->add(enemyNode);
	mRealTypeCount[TEKITYPE_C]++;
	mTotalCount++;
}


void RandEnemyUnit::setEnemyTypeI(int iteration)
{
	if (mRealTypeCount[iteration] < mRealTypeMax[iteration]) {
		if (mGenerator->mIsVersusMode) {
			setVersusEnemyTypeI(iteration);
		}
		if (mRealTypeCount[iteration] < mRealTypeMax[iteration]) {
			for (int i = 0; i < 100; i++) {
				setSlotEnemyTypeI(-1, iteration);
				setUnitRandEnemyTypeI(iteration);
				if (mMapTile && mSpawn && mEnemyUnit && iteration) {
					makeSetEnemyTypeI(mMapTile, mSpawn, mEnemyUnit, iteration);
					if (mRealTypeCount[iteration] < mRealTypeMax[iteration]) {
						continue;
					}
					return;
				}
				return;
			}
		}
	}
}

void RandEnemyUnit::setVersusHibaTypeI(int iteration)
{
	int count = 0;
	FOREACH_NODE(EnemyNode, mGenerator->mMainEnemies->mChild, currEnemy)
	{
		TekiInfo* info = currEnemy->getTekiInfo();
		if (info && info->mType == enemyTypes[iteration]) {
			count += info->mWeight / 10;
			if (count > mRealTypeCount[iteration]) {
				int altNum     = (count - mRealTypeCount[iteration]) % 2;
				int roundedMax = ((count - mRealTypeCount[iteration]) / 2) * 2;

				int randIdx = randInt(2);
				for (int i = 0; i < roundedMax; i++, randIdx ^= 1) {
					setSlotEnemyTypeB(randIdx);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeB(mMapTile, mSpawn, currEnemy->mEnemyUnit);
						continue;
					}
					return;
				}

				if (altNum) {
					int slot = -1;
					setSlotEnemyTypeB(slot);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeB(mMapTile, mSpawn, currEnemy->mEnemyUnit);
						continue;
					}
					return;
				}
			}
		}
	}
}

void RandEnemyUnit::setVersusEnemyTypeI(int iteration)
{
	int count = 0;
	FOREACH_NODE(EnemyNode, mGenerator->mMainEnemies->mChild, currEnemy)
	{
		TekiInfo* info = currEnemy->getTekiInfo();
		if (info && info->mType == enemyTypes[iteration]) {
			count += info->mWeight / 10;
			if (count > mRealTypeCount[iteration]) {
				int altNum     = (count - mRealTypeCount[iteration]) % 2;
				int roundedMax = ((count - mRealTypeCount[iteration]) / 2) * 2;

				int randIdx = randInt(2);
				for (int i = 0; i < roundedMax; i++, randIdx ^= 1) {
					int slot = -1;
					setSlotEnemyTypeI(randIdx, iteration);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeI(mMapTile, mSpawn, currEnemy->mEnemyUnit, iteration);
						continue;
					}
					return;
				}

				if (altNum) {
					int slot = -1;
					setSlotEnemyTypeI(slot, iteration);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeI(mMapTile, mSpawn, currEnemy->mEnemyUnit, iteration);
						continue;
					}
					return;
				}
			}
		}
	}
}

void RandEnemyUnit::setSlotEnemyTypeI(int p1, int iteration)
{
	MapNode* nodeList[128];
	BaseGen* spawnList[128];
	int scoreList[128];
	Vector3f vecArray[3];
	f32 floatArray[3] = { 300.0f, 150.0f, 150.0f }; // 0x2C

	int counter      = 0;
	int vsScore      = 0;
	int vsSign       = 0;
	int spawnCounter = 0;
	int scoreTally   = 0;

	MapNode* placedNodes = mGenerator->getPlacedNodes();
	if (mGenerator->mIsVersusMode) {
		MapNode* onyon;
		for (int i = FIXNODE_VsStart; i <= FIXNODE_VsEnd; i++) {
			onyon               = mMapScore->getFixObjNode(i);
			BaseGen* onyonSpawn = mMapScore->getFixObjGen(i);
			if (!onyon) {
				continue;
			}

			Vector3f spawnPos   = onyon->getBaseGenGlobalPosition(onyonSpawn);
			vecArray[counter]   = spawnPos;
			floatArray[counter] = 400.0f;

			if (p1 == 0 && counter == 0) {
				vsScore = onyon->getVersusScore();
				vsSign  = -1;
			} else if (p1 == 1 && counter == 1) {
				vsScore = onyon->getVersusScore();
				vsSign  = 1;
			}
			counter++;
		}
	} else {
		MapNode* exit;
		// loop through start and exits (pod, hole, fountain)
		for (int i = FIXNODE_Pod; i <= FIXNODE_Fountain; i++) {
			exit               = mMapScore->getFixObjNode(i);
			BaseGen* exitSpawn = mMapScore->getFixObjGen(i);
			if (exit) {
				Vector3f spawnPos = exit->getBaseGenGlobalPosition(exitSpawn);
				vecArray[counter] = spawnPos;
				counter++;
			}
		}
	}

	FOREACH_NODE(MapNode, placedNodes->mChild, node)
	{
		if (node->mUnitInfo->getUnitKind() != UNITKIND_Room) {
			continue;
		}

		BaseGen* spawnRoot = node->mUnitInfo->getBaseGen();
		if (!spawnRoot) {
			continue;
		}

		FOREACH_NODE(BaseGen, spawnRoot->mChild, spawn)
		{
			if (spawn->mSpawnType != enemyTypes[iteration]) {
				continue;
			}

			if (isEnemySetGen(node, spawn)) {
				continue;
			}

			bool check = true;
			for (int i = 0; i < counter; i++) {
				if (check) {
					Vector3f spawnPos = node->getBaseGenGlobalPosition(spawn);
					if (spawnPos.distance(vecArray[i]) < floatArray[i]) {
						check = false;
					}
				}
			}

			if (check) {
				nodeList[spawnCounter]  = node;
				spawnList[spawnCounter] = spawn;
				scoreList[spawnCounter] = vsSign * (vsScore + nodeList[spawnCounter]->getVersusScore());
				if (scoreList[spawnCounter] <= 0) {
					scoreList[spawnCounter] = 1;
				}

				scoreTally += scoreList[spawnCounter];
				spawnCounter++;
			}
		}
	}

	mMapTile = nullptr;
	mSpawn   = nullptr;

	if (spawnCounter == 0) {
		return;
	}

	int randScoreThreshold = (f32)scoreTally * randFloat();
	int scoreCounter       = 0;
	for (int i = 0; i < spawnCounter; i++) {
		scoreCounter += scoreList[i];
		if (scoreCounter > randScoreThreshold) {
			mMapTile = nodeList[i];
			mSpawn   = spawnList[i];
			return;
		}
	}
}

void RandEnemyUnit::setUnitRandEnemyTypeI(int iteration)
{
	EnemyUnit* enemyList[128];
	int weightList[128];

	int counter     = 0;
	int weightTally = 0;

	EnemyUnit* enemy;
	int weightCounter = 0;
	EnemyNode* node;

	for (node = static_cast<EnemyNode*>(mGenerator->mMainEnemies->mChild); node; node = static_cast<EnemyNode*>(node->mNext)) {
		enemy = node->mEnemyUnit;
		if (enemy->mTekiInfo && enemy->mTekiInfo->mType == enemyTypes[iteration]) {
			int ones = enemy->mTekiInfo->mWeight % 10;
			int tens = enemy->mTekiInfo->mWeight / 10;
			if (tens) {
				weightCounter += tens;
				if (weightCounter > mRealTypeCount[iteration]) {
					mEnemyUnit = enemy;
					return;
				}
			}

			if (ones) {
				enemyList[counter]  = enemy;
				weightList[counter] = ones;
				weightTally += weightList[counter];
				counter++;
			}
		}
	}

	mEnemyUnit = nullptr;
	if (weightTally == 0) {
		return;
	}

	int randWeightThreshold = (f32)weightTally * randFloat();
	int ctr                 = 0;
	for (int i = 0; i < counter; i++) {
		ctr += weightList[i];
		if (ctr > randWeightThreshold) {
			mEnemyUnit = enemyList[i];
			return;
		}
	}
}

void RandEnemyUnit::makeSetEnemyTypeI(MapNode* tile, BaseGen* spawn, EnemyUnit* enemy, int iteration)
{
	EnemyNode* enemyNode = new EnemyNode(enemy, spawn, 1);
	enemyNode->makeGlobalData(tile);
	tile->mEnemyNode->add(enemyNode);
	mRealTypeCount[iteration]++;
	mTotalCount++;
}

void RandEnemyUnit::setEnemyTypeF()
{
	if (mRealTypeCount[TEKITYPE_F] < mRealTypeMax[TEKITYPE_F]) {
		if (mGenerator->mIsVersusMode) {
			setVersusEnemyTypeF();
		}
		if (mRealTypeCount[TEKITYPE_F] < mRealTypeMax[TEKITYPE_F]) {
			for (int i = 0; i < 100; i++) {
				setSlotEnemyTypeF(-1);
				setUnitRandEnemyTypeF();
				if (mMapTile && mSpawn && mEnemyUnit) {
					makeSetEnemyTypeF(mMapTile, mSpawn, mEnemyUnit);
					if (mRealTypeCount[TEKITYPE_F] < mRealTypeMax[TEKITYPE_F]) {
						continue;
					}
					return;
				}
				return;
			}
		}
	}
}

void RandEnemyUnit::setVersusEnemyTypeF()
{
	int count = 0;
	FOREACH_NODE(EnemyNode, mGenerator->mMainEnemies->mChild, currEnemy)
	{
		TekiInfo* info = currEnemy->getTekiInfo();
		if (info && info->mType == BaseGen::CGT_EnemySpecial) {
			count += info->mWeight / 10;
			if (count > mRealTypeCount[TEKITYPE_F]) {
				int altNum     = (count - mRealTypeCount[TEKITYPE_F]) % 2;
				int roundedMax = ((count - mRealTypeCount[TEKITYPE_F]) / 2) * 2;

				int randIdx = randInt(2);
				for (int i = 0; i < roundedMax; i++, randIdx ^= 1) {
					int slot = -1;
					setSlotEnemyTypeF(randIdx);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeF(mMapTile, mSpawn, currEnemy->mEnemyUnit);
						continue;
					}
					return;
				}

				if (altNum) {
					int slot = -1;
					setSlotEnemyTypeF(slot);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeF(mMapTile, mSpawn, currEnemy->mEnemyUnit);
						continue;
					}
					return;
				}
			}
		}
	}
}

void RandEnemyUnit::setSlotEnemyTypeF(int p1)
{
	MapNode* nodeList[128];
	BaseGen* spawnList[128];
	int scoreList[128];
	Vector3f vecArray[3];
	f32 floatArray[3] = { 300.0f, 150.0f, 150.0f }; // 0x2C

	int counter      = 0;
	int vsScore      = 0;
	int vsSign       = 0;
	int spawnCounter = 0;
	int scoreTally   = 0;

	MapNode* placedNodes = mGenerator->getPlacedNodes();
	if (mGenerator->mIsVersusMode) {
		MapNode* onyon;
		for (int i = FIXNODE_VsStart; i <= FIXNODE_VsEnd; i++) {
			onyon               = mMapScore->getFixObjNode(i);
			BaseGen* onyonSpawn = mMapScore->getFixObjGen(i);
			if (!onyon) {
				continue;
			}

			Vector3f spawnPos   = onyon->getBaseGenGlobalPosition(onyonSpawn);
			vecArray[counter]   = spawnPos;
			floatArray[counter] = 400.0f;

			if (p1 == 0 && counter == 0) {
				vsScore = onyon->getVersusScore();
				vsSign  = -1;
			} else if (p1 == 1 && counter == 1) {
				vsScore = onyon->getVersusScore();
				vsSign  = 1;
			}
			counter++;
		}
	} else {
		MapNode* exit;
		// loop through start and exits (pod, hole, fountain)
		for (int i = FIXNODE_Pod; i <= FIXNODE_Fountain; i++) {
			exit               = mMapScore->getFixObjNode(i);
			BaseGen* exitSpawn = mMapScore->getFixObjGen(i);
			if (exit) {
				Vector3f spawnPos = exit->getBaseGenGlobalPosition(exitSpawn);
				vecArray[counter] = spawnPos;
				counter++;
			}
		}
	}

	FOREACH_NODE(MapNode, placedNodes->mChild, node)
	{
		if (node->mUnitInfo->getUnitKind() != UNITKIND_Room) {
			continue;
		}

		BaseGen* spawnRoot = node->mUnitInfo->getBaseGen();
		if (!spawnRoot) {
			continue;
		}

		FOREACH_NODE(BaseGen, spawnRoot->mChild, spawn)
		{
			if (spawn->mSpawnType != BaseGen::CGT_EnemySpecial) {
				continue;
			}

			if (isEnemySetGen(node, spawn)) {
				continue;
			}

			bool check = true;
			for (int i = 0; i < counter; i++) {
				if (check) {
					Vector3f spawnPos = node->getBaseGenGlobalPosition(spawn);
					if (spawnPos.distance(vecArray[i]) < floatArray[i]) {
						check = false;
					}
				}
			}

			if (check) {
				nodeList[spawnCounter]  = node;
				spawnList[spawnCounter] = spawn;
				scoreList[spawnCounter] = vsSign * (vsScore + nodeList[spawnCounter]->getVersusScore());
				if (scoreList[spawnCounter] <= 0) {
					scoreList[spawnCounter] = 1;
				}

				scoreTally += scoreList[spawnCounter];
				spawnCounter++;
			}
		}
	}

	mMapTile = nullptr;
	mSpawn   = nullptr;

	if (spawnCounter == 0) {
		return;
	}

	int randScoreThreshold = (f32)scoreTally * randFloat();
	int scoreCounter       = 0;
	for (int i = 0; i < spawnCounter; i++) {
		scoreCounter += scoreList[i];
		if (scoreCounter > randScoreThreshold) {
			mMapTile = nodeList[i];
			mSpawn   = spawnList[i];
			return;
		}
	}
}

void RandEnemyUnit::setUnitRandEnemyTypeF()
{
	EnemyUnit* enemyList[128];
	int weightList[128];

	int counter     = 0;
	int weightTally = 0;

	EnemyUnit* enemy;
	int weightCounter = 0;
	EnemyNode* node;

	for (node = static_cast<EnemyNode*>(mGenerator->mMainEnemies->mChild); node; node = static_cast<EnemyNode*>(node->mNext)) {
		enemy = node->mEnemyUnit;
		if (enemy->mTekiInfo && enemy->mTekiInfo->mType == BaseGen::CGT_EnemySpecial) {
			int ones = enemy->mTekiInfo->mWeight % 10;
			int tens = enemy->mTekiInfo->mWeight / 10;
			if (tens) {
				weightCounter += tens;
				if (weightCounter > mRealTypeCount[TEKITYPE_F]) {
					mEnemyUnit = enemy;
					return;
				}
			}

			if (ones) {
				enemyList[counter]  = enemy;
				weightList[counter] = ones;
				weightTally += weightList[counter];
				counter++;
			}
		}
	}

	mEnemyUnit = nullptr;
	if (weightTally == 0) {
		return;
	}

	int randWeightThreshold = (f32)weightTally * randFloat();
	int ctr                 = 0;
	for (int i = 0; i < counter; i++) {
		ctr += weightList[i];
		if (ctr > randWeightThreshold) {
			mEnemyUnit = enemyList[i];
			return;
		}
	}
}

void RandEnemyUnit::makeSetEnemyTypeF(MapNode* tile, BaseGen* spawn, EnemyUnit* enemy)
{
	EnemyNode* enemyNode = new EnemyNode(enemy, spawn, 1);
	enemyNode->makeGlobalData(tile);
	tile->mEnemyNode->add(enemyNode);
	mRealTypeCount[TEKITYPE_F]++;
	mTotalCount++;
}

void RandEnemyUnit::setEnemyTypeB()
{
	if (mRealTypeCount[TEKITYPE_B] < mRealTypeMax[TEKITYPE_B]) {
		if (mGenerator->mIsVersusMode) {
			setVersusEnemyTypeB();
		}
		if (mRealTypeCount[TEKITYPE_B] < mRealTypeMax[TEKITYPE_B]) {
			for (int i = 0; i < 100; i++) {
				setSlotEnemyTypeB(-1);
				setUnitRandEnemyTypeB();
				if (mMapTile && mSpawn && mEnemyUnit) {
					makeSetEnemyTypeB(mMapTile, mSpawn, mEnemyUnit);
					if (mRealTypeCount[TEKITYPE_B] < mRealTypeMax[TEKITYPE_B]) {
						continue;
					}
					return;
				}
				return;
			}
		}
	}
}

void RandEnemyUnit::setVersusEnemyTypeB()
{
	int count = 0;
	FOREACH_NODE(EnemyNode, mGenerator->mMainEnemies->mChild, currEnemy)
	{
		TekiInfo* info = currEnemy->getTekiInfo();
		if (info && info->mType == BaseGen::CGT_EnemyHard) {
			count += info->mWeight / 10;
			if (count > mRealTypeCount[TEKITYPE_B]) {
				int altNum     = (count - mRealTypeCount[TEKITYPE_B]) % 2;
				int roundedMax = ((count - mRealTypeCount[TEKITYPE_B]) / 2) * 2;

				int randIdx = randInt(2);
				for (int i = 0; i < roundedMax; i++, randIdx ^= 1) {
					setSlotEnemyTypeB(randIdx);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeB(mMapTile, mSpawn, currEnemy->mEnemyUnit);
						continue;
					}
					return;
				}

				if (altNum) {
					int slot = -1;
					setSlotEnemyTypeB(slot);

					if (mMapTile && mSpawn) {
						makeSetEnemyTypeB(mMapTile, mSpawn, currEnemy->mEnemyUnit);
						continue;
					}
					return;
				}
			}
		}
	}
}

void RandEnemyUnit::setSlotEnemyTypeB(int p1)
{
	MapNode* nodeList[128];
	BaseGen* spawnList[128];
	int scoreList[128];
	Vector3f vecArray[3];
	f32 floatArray[3] = { 300.0f, 150.0f, 150.0f }; // 0x2C

	int counter      = 0;
	int vsScore      = 0;
	int vsSign       = 0;
	int spawnCounter = 0;
	int scoreTally   = 0;

	MapNode* placedNodes = mGenerator->getPlacedNodes();
	if (mGenerator->mIsVersusMode) {
		MapNode* onyon;
		BaseGen* onyonSpawn;
		for (int i = FIXNODE_VsStart; i <= FIXNODE_VsEnd; i++) {
			onyon      = mMapScore->getFixObjNode(i);
			onyonSpawn = mMapScore->getFixObjGen(i);
			if (!onyon) {
				continue;
			}

			Vector3f spawnPos   = onyon->getBaseGenGlobalPosition(onyonSpawn);
			vecArray[counter]   = spawnPos;
			floatArray[counter] = 400.0f;

			if (p1 == 0 && counter == 0) {
				vsScore = onyon->getVersusScore();
				vsSign  = -1;
			} else if (p1 == 1 && counter == 1) {
				vsScore = onyon->getVersusScore();
				vsSign  = 1;
			}
			counter++;
		}
	} else {
		MapNode* exit;
		// loop through start and exits (pod, hole, fountain)
		for (int i = FIXNODE_Pod; i <= FIXNODE_Fountain; i++) {
			exit               = mMapScore->getFixObjNode(i);
			BaseGen* exitSpawn = mMapScore->getFixObjGen(i);
			if (exit) {
				Vector3f spawnPos = exit->getBaseGenGlobalPosition(exitSpawn);
				vecArray[counter] = spawnPos;
				counter++;
			}
		}
	}

	FOREACH_NODE(MapNode, placedNodes->mChild, node)
	{
		if (node->mUnitInfo->getUnitKind() != UNITKIND_Room) {
			continue;
		}

		BaseGen* spawnRoot = node->mUnitInfo->getBaseGen();
		if (!spawnRoot) {
			continue;
		}

		FOREACH_NODE(BaseGen, spawnRoot->mChild, spawn)
		{
			if (spawn->mSpawnType != BaseGen::CGT_EnemyHard) {
				continue;
			}

			if (isEnemySetGen(node, spawn)) {
				continue;
			}

			bool check = true;
			for (int i = 0; i < counter; i++) {
				if (check) {
					Vector3f spawnPos = node->getBaseGenGlobalPosition(spawn);
					if (spawnPos.distance(vecArray[i]) < floatArray[i]) {
						check = false;
					}
				}
			}

			if (check) {
				nodeList[spawnCounter]  = node;
				spawnList[spawnCounter] = spawn;
				scoreList[spawnCounter] = vsSign * (vsScore + nodeList[spawnCounter]->getVersusScore());
				if (scoreList[spawnCounter] <= 0) {
					scoreList[spawnCounter] = 1;
				}

				scoreTally += scoreList[spawnCounter];
				spawnCounter++;
			}
		}
	}

	mMapTile = nullptr;
	mSpawn   = nullptr;

	if (spawnCounter == 0) {
		return;
	}

	int randScoreThreshold = (f32)scoreTally * randFloat();
	int scoreCounter       = 0;
	for (int i = 0; i < spawnCounter; i++) {
		scoreCounter += scoreList[i];
		if (scoreCounter > randScoreThreshold) {
			mMapTile = nodeList[i];
			mSpawn   = spawnList[i];
			return;
		}
	}
}

void RandEnemyUnit::setUnitRandEnemyTypeB()
{
	EnemyUnit* enemyList[128];
	int weightList[128];

	int counter     = 0;
	int weightTally = 0;

	EnemyUnit* enemy;
	int weightCounter = 0;
	EnemyNode* node;

	for (node = static_cast<EnemyNode*>(mGenerator->mMainEnemies->mChild); node; node = static_cast<EnemyNode*>(node->mNext)) {
		enemy = node->mEnemyUnit;
		if (enemy->mTekiInfo && enemy->mTekiInfo->mType == BaseGen::CGT_EnemyHard) {
			int ones = enemy->mTekiInfo->mWeight % 10;
			int tens = enemy->mTekiInfo->mWeight / 10;
			if (tens) {
				weightCounter += tens;
				if (weightCounter > mRealTypeCount[TEKITYPE_B]) {
					mEnemyUnit = enemy;
					return;
				}
			}

			if (ones) {
				enemyList[counter]  = enemy;
				weightList[counter] = ones;
				weightTally += weightList[counter];
				counter++;
			}
		}
	}

	mEnemyUnit = nullptr;
	if (weightTally == 0) {
		return;
	}

	int randWeightThreshold = (f32)weightTally * randFloat();
	int ctr                 = 0;
	for (int i = 0; i < counter; i++) {
		ctr += weightList[i];
		if (ctr > randWeightThreshold) {
			mEnemyUnit = enemyList[i];
			return;
		}
	}
}

void RandEnemyUnit::makeSetEnemyTypeB(MapNode* tile, BaseGen* spawn, EnemyUnit* enemy)
{
	EnemyNode* enemyNode = new EnemyNode(enemy, spawn, 1);
	enemyNode->makeGlobalData(tile);
	tile->mEnemyNode->add(enemyNode);
	mRealTypeCount[TEKITYPE_B]++;
	mTotalCount++;
}

void RandEnemyUnit::setEnemyTypeA()
{
	if (mRealTypeCount[TEKITYPE_A] < mRealTypeMax[TEKITYPE_A]) {
		if (mGenerator->mIsVersusMode) {
			setVersusEasyEnemy();
			setVersusEnemyTypeA();
		}
		if (mRealTypeCount[TEKITYPE_A] < mRealTypeMax[TEKITYPE_A]) {
			for (int i = 0; i < 100; i++) {
				int max   = 0;
				int min   = 0;
				int count = 0;
				setSlotEnemyTypeA(max, min, -1);
				setUnitRandEnemyTypeA(count, max, min);
				if (mMapTile && mSpawn && mEnemyUnit && count) {
					makeSetEnemyTypeA(mMapTile, mSpawn, mEnemyUnit, count);
					if (mRealTypeCount[TEKITYPE_A] < mRealTypeMax[TEKITYPE_A]) {
						continue;
					}
					return;
				}
				return;
			}
		}
	}
}

void RandEnemyUnit::setVersusEnemyTypeA()
{
	int count = 0;
	FOREACH_NODE(EnemyNode, mGenerator->mMainEnemies->mChild, currEnemy)
	{
		TekiInfo* info = currEnemy->getTekiInfo();
		if (info && info->mType == BaseGen::CGT_EnemyEasy) {
			count += info->mWeight / 10;
			if (count > mRealTypeCount[TEKITYPE_A]) {
				int max = (count - mRealTypeCount[TEKITYPE_A]);

				int vsColor = randInt(2);

				for (int i = 0; i < max; i++, vsColor ^= 1) {
					if (count <= mRealTypeCount[TEKITYPE_A]) {
						continue;
					}
					int max = 0;
					int min = 0;
					setSlotEnemyTypeA(max, min, vsColor);

					max = (max < count - mRealTypeCount[TEKITYPE_A]) ? max : count - mRealTypeCount[TEKITYPE_A];

					int enemiesToMake = (max <= min) ? max : min + randInt(max - min + 1);

					if (mMapTile && mSpawn && enemiesToMake) {
						makeSetEnemyTypeA(mMapTile, mSpawn, currEnemy->mEnemyUnit, enemiesToMake);
						continue;
					}
					return;
				}
			}
		}
	}
}

void RandEnemyUnit::setSlotEnemyTypeA(int& max, int& min, int vsColor)
{
	MapNode* nodeList[128];
	BaseGen* spawnList[128];
	int scoreList[128];
	Vector3f vecArray[2];
	f32 floatArray[2] = { 400.0f, 400.0f }; // 0x2C

	int counter      = 0;
	int vsScore      = 0;
	int vsSign       = 0;
	int spawnCounter = 0;
	int scoreTally   = 0;

	MapNode* placedNodes = mGenerator->getPlacedNodes();
	if (mGenerator->mIsVersusMode) {
		MapNode* onyon;
		BaseGen* onyonSpawn;
		for (int i = FIXNODE_VsStart; i <= FIXNODE_VsEnd; i++) {
			onyon      = mMapScore->getFixObjNode(i);
			onyonSpawn = mMapScore->getFixObjGen(i);
			if (!onyon) {
				continue;
			}

			Vector3f spawnPos = onyon->getBaseGenGlobalPosition(onyonSpawn);
			vecArray[counter] = spawnPos;

			if (vsColor == Blue && counter == 0) {
				vsScore = onyon->getVersusScore();
				vsSign  = -1;
			} else if (vsColor == Red && counter == 1) {
				vsScore = onyon->getVersusScore();
				vsSign  = 1;
			}
			counter++;
		}
	} else {
		MapNode* start      = mMapScore->getFixObjNode(FIXNODE_Pod);
		BaseGen* startSpawn = mMapScore->getFixObjGen(FIXNODE_Pod);
		if (start) {
			Vector3f spawnPos   = start->getBaseGenGlobalPosition(startSpawn);
			vecArray[counter]   = spawnPos;
			floatArray[counter] = 300.0f;
			counter++;
		}
	}

	FOREACH_NODE(MapNode, placedNodes->mChild, node)
	{
		if (node->mUnitInfo->getUnitKind() != UNITKIND_Room) {
			continue;
		}

		BaseGen* spawnRoot = node->mUnitInfo->getBaseGen();
		if (!spawnRoot) {
			continue;
		}

		FOREACH_NODE(BaseGen, spawnRoot->mChild, spawn)
		{
			if (spawn->mSpawnType != BaseGen::CGT_EnemyEasy) {
				continue;
			}

			if (isEnemySetGen(node, spawn)) {
				continue;
			}

			bool check = true;
			for (int i = 0; i < counter; i++) {
				if (check) {
					Vector3f spawnPos = node->getBaseGenGlobalPosition(spawn);
					if (spawnPos.distance(vecArray[i]) < floatArray[i]) {
						check = false;
					}
				}
			}

			if (check) {
				nodeList[spawnCounter]  = node;
				spawnList[spawnCounter] = spawn;
				scoreList[spawnCounter] = vsSign * (vsScore + nodeList[spawnCounter]->getVersusScore());
				if (scoreList[spawnCounter] <= 0) {
					scoreList[spawnCounter] = 1;
				}

				scoreTally += scoreList[spawnCounter];
				spawnCounter++;
			}
		}
	}

	mMapTile = nullptr;
	mSpawn   = nullptr;

	if (spawnCounter == 0) {
		return;
	}

	int randScoreThreshold = (f32)scoreTally * randFloat();
	int scoreCounter       = 0;
	for (int i = 0; i < spawnCounter; i++) {
		scoreCounter += scoreList[i];
		if (scoreCounter > randScoreThreshold) {
			mMapTile = nodeList[i];
			mSpawn   = spawnList[i];

			max = spawnList[i]->mMaximum;
			min = spawnList[i]->mMinimum;
			return;
		}
	}
}

void RandEnemyUnit::setUnitRandEnemyTypeA(int& count, int max, int min)
{
	EnemyUnit* enemyList[128];
	int weightList[128];

	int counter     = 0;
	int weightTally = 0;

	EnemyUnit* enemy;
	int weightCounter = 0;
	EnemyNode* node;

	for (node = static_cast<EnemyNode*>(mGenerator->mMainEnemies->mChild); node; node = static_cast<EnemyNode*>(node->mNext)) {
		enemy = node->mEnemyUnit;
		if (enemy->mTekiInfo && enemy->mTekiInfo->mType == BaseGen::CGT_EnemyEasy) {
			int ones = enemy->mTekiInfo->mWeight % 10;
			int tens = enemy->mTekiInfo->mWeight / 10;
			if (tens) {
				weightCounter += tens;
				if (weightCounter > mRealTypeCount[TEKITYPE_A]) {
					mEnemyUnit = enemy;

					int goalAmt = weightCounter - mRealTypeCount[TEKITYPE_A];
					if (max < goalAmt) {
						goalAmt = max;
					}
					if (goalAmt <= min) {
						count = goalAmt;
						return;
					}

					count = min + randInt(goalAmt - min + 1);
					return;
				}
			}

			if (ones) {
				enemyList[counter]  = enemy;
				weightList[counter] = ones;
				weightTally += weightList[counter];
				counter++;
			}
		}
	}

	mEnemyUnit = nullptr;
	if (weightTally == 0) {
		return;
	}

	int randWeightThreshold = (f32)weightTally * randFloat();
	int ctr                 = 0;
	for (int i = 0; i < counter; i++) {
		ctr += weightList[i];
		if (ctr > randWeightThreshold) {
			mEnemyUnit = enemyList[i];

			int goalAmt = mMaxEnemies - mTotalCount;
			if (max < goalAmt) {
				goalAmt = max;
			}

			if (goalAmt <= min) {
				count = goalAmt;
				return;
			}

			count = min + randInt(goalAmt - min + 1);
			return;
		}
	}
}

void RandEnemyUnit::makeSetEnemyTypeA(MapNode* tile, BaseGen* spawn, EnemyUnit* enemy, int count)
{
	Vector3f vecArray[16];
	Vector3f spawnPos = tile->getBaseGenGlobalPosition(spawn);
	f32 radius        = spawn->mRadius;
	for (int i = 0; i < count; i++) {
		f32 randDist  = randWeightFloat(radius);
		f32 randAngle = randWeightFloat(TAU);
		vecArray[i].x = randDist * sinf(randAngle) + spawnPos.x;
		vecArray[i].y = spawnPos.y;
		vecArray[i].z = randDist * cosf(randAngle) + spawnPos.z;
	}

	for (int i = 0; i < 5; i++) {             // r5
		for (int j = 0; j < count; j++) {     // r6
			for (int k = 0; k < count; k++) { // r7
				if (j == k) {
					continue;
				}
				// issues are here
				Vector3f sep = vecArray[j];
				sep          = sep - vecArray[k];
				f32 dist     = vecArray[j].distance(vecArray[k]);
				if (dist < 35.0f) {
					sep.normalise();
					sep *= (0.5f * (35.0f - dist));
					vecArray[j] += sep;
					vecArray[k] -= sep;
				}
			}
		}
	}

	for (int i = 0; i < count; i++) {
		EnemyNode* enemyNode = new EnemyNode(enemy, spawn, 1);
		f32 dir              = JMAAtan2Radian(vecArray[i].x - spawnPos.x, vecArray[i].z - spawnPos.z);
		enemyNode->setGlobalData(vecArray[i], dir);
		tile->mEnemyNode->add(enemyNode);
	}

	mRealTypeCount[TEKITYPE_A] += count;
	mTotalCount += count;
}

void RandEnemyUnit::setEnemyTypeJ(int iter)
{
	if (mRealTypeCount[iter] < mRealTypeMax[iter]) {
		if (mRealTypeCount[iter] < mRealTypeMax[iter]) {
			for (int i = 0; i < 100; i++) {
				int max   = 0;
				int min   = 0;
				int count = 0;
				setSlotEnemyTypeJ(max, min, -1, iter);
				setUnitRandEnemyTypeJ(count, max, min, iter);
				if (mMapTile && mSpawn && mEnemyUnit && count) {
					makeSetEnemyTypeJ(mMapTile, mSpawn, mEnemyUnit, count, iter);
					if (mRealTypeCount[iter] < mRealTypeMax[iter]) {
						continue;
					}
					return;
				}
				return;
			}
		}
	}
}

void RandEnemyUnit::setSlotEnemyTypeJ(int& max, int& min, int vsColor, int iter)
{
	MapNode* nodeList[128];
	BaseGen* spawnList[128];
	int scoreList[128];
	Vector3f vecArray[2];
	f32 floatArray[2] = { 400.0f, 400.0f }; // 0x2C

	int counter      = 0;
	int vsScore      = 0;
	int vsSign       = 0;
	int spawnCounter = 0;
	int scoreTally   = 0;

	MapNode* placedNodes = mGenerator->getPlacedNodes();
	if (mGenerator->mIsVersusMode) {
		MapNode* onyon;
		BaseGen* onyonSpawn;
		for (int i = FIXNODE_VsStart; i <= FIXNODE_VsEnd; i++) {
			onyon      = mMapScore->getFixObjNode(i);
			onyonSpawn = mMapScore->getFixObjGen(i);
			if (!onyon) {
				continue;
			}

			Vector3f spawnPos = onyon->getBaseGenGlobalPosition(onyonSpawn);
			vecArray[counter] = spawnPos;

			if (vsColor == Blue && counter == 0) {
				vsScore = onyon->getVersusScore();
				vsSign  = -1;
			} else if (vsColor == Red && counter == 1) {
				vsScore = onyon->getVersusScore();
				vsSign  = 1;
			}
			counter++;
		}
	} else {
		MapNode* start      = mMapScore->getFixObjNode(FIXNODE_Pod);
		BaseGen* startSpawn = mMapScore->getFixObjGen(FIXNODE_Pod);
		if (start) {
			Vector3f spawnPos   = start->getBaseGenGlobalPosition(startSpawn);
			vecArray[counter]   = spawnPos;
			floatArray[counter] = 300.0f;
			counter++;
		}
	}

	FOREACH_NODE(MapNode, placedNodes->mChild, node)
	{
		if (node->mUnitInfo->getUnitKind() != UNITKIND_Room) {
			continue;
		}

		BaseGen* spawnRoot = node->mUnitInfo->getBaseGen();
		if (!spawnRoot) {
			continue;
		}

		FOREACH_NODE(BaseGen, spawnRoot->mChild, spawn)
		{
			if (spawn->mSpawnType != enemyTypes[iter]) {
				continue;
			}

			if (isEnemySetGen(node, spawn)) {
				continue;
			}

			bool check = true;
			for (int i = 0; i < counter; i++) {
				if (check) {
					Vector3f spawnPos = node->getBaseGenGlobalPosition(spawn);
					if (spawnPos.distance(vecArray[i]) < floatArray[i]) {
						check = false;
					}
				}
			}

			if (check) {
				nodeList[spawnCounter]  = node;
				spawnList[spawnCounter] = spawn;
				scoreList[spawnCounter] = vsSign * (vsScore + nodeList[spawnCounter]->getVersusScore());
				if (scoreList[spawnCounter] <= 0) {
					scoreList[spawnCounter] = 1;
				}

				scoreTally += scoreList[spawnCounter];
				spawnCounter++;
			}
		}
	}

	mMapTile = nullptr;
	mSpawn   = nullptr;

	if (spawnCounter == 0) {
		return;
	}

	int randScoreThreshold = (f32)scoreTally * randFloat();
	int scoreCounter       = 0;
	for (int i = 0; i < spawnCounter; i++) {
		scoreCounter += scoreList[i];
		if (scoreCounter > randScoreThreshold) {
			mMapTile = nodeList[i];
			mSpawn   = spawnList[i];

			max = spawnList[i]->mMaximum;
			min = spawnList[i]->mMinimum;
			return;
		}
	}
}

void RandEnemyUnit::setUnitRandEnemyTypeJ(int& count, int max, int min, int iter)
{
	EnemyUnit* enemyList[128];
	int weightList[128];

	int counter     = 0;
	int weightTally = 0;

	EnemyUnit* enemy;
	int weightCounter = 0;
	EnemyNode* node;

	for (node = static_cast<EnemyNode*>(mGenerator->mMainEnemies->mChild); node; node = static_cast<EnemyNode*>(node->mNext)) {
		enemy = node->mEnemyUnit;
		if (enemy->mTekiInfo && enemy->mTekiInfo->mType == enemyTypes[iter]) {
			int ones = enemy->mTekiInfo->mWeight % 10;
			int tens = enemy->mTekiInfo->mWeight / 10;
			if (tens) {
				weightCounter += tens;
				if (weightCounter > mRealTypeCount[iter]) {
					mEnemyUnit = enemy;

					int goalAmt = weightCounter - mRealTypeCount[iter];
					if (max < goalAmt) {
						goalAmt = max;
					}
					if (goalAmt <= min) {
						count = goalAmt;
						return;
					}

					count = min + randInt(goalAmt - min + 1);
					return;
				}
			}

			if (ones) {
				enemyList[counter]  = enemy;
				weightList[counter] = ones;
				weightTally += weightList[counter];
				counter++;
			}
		}
	}

	mEnemyUnit = nullptr;
	if (weightTally == 0) {
		return;
	}

	int randWeightThreshold = (f32)weightTally * randFloat();
	int ctr                 = 0;
	for (int i = 0; i < counter; i++) {
		ctr += weightList[i];
		if (ctr > randWeightThreshold) {
			mEnemyUnit = enemyList[i];

			int goalAmt = mMaxEnemies - mTotalCount;
			if (max < goalAmt) {
				goalAmt = max;
			}

			if (goalAmt <= min) {
				count = goalAmt;
				return;
			}

			count = min + randInt(goalAmt - min + 1);
			return;
		}
	}
}

void RandEnemyUnit::makeSetEnemyTypeJ(MapNode* tile, BaseGen* spawn, EnemyUnit* enemy, int count, int iter)
{
	Vector3f vecArray[16];
	Vector3f spawnPos = tile->getBaseGenGlobalPosition(spawn);
	f32 radius        = spawn->mRadius;
	for (int i = 0; i < count; i++) {
		f32 randDist  = randWeightFloat(radius);
		f32 randAngle = randWeightFloat(TAU);
		vecArray[i].x = randDist * sinf(randAngle) + spawnPos.x;
		vecArray[i].y = spawnPos.y;
		vecArray[i].z = randDist * cosf(randAngle) + spawnPos.z;
	}

	for (int i = 0; i < 5; i++) {             // r5
		for (int j = 0; j < count; j++) {     // r6
			for (int k = 0; k < count; k++) { // r7
				if (j == k) {
					continue;
				}
				// issues are here
				Vector3f sep = vecArray[j];
				sep          = sep - vecArray[k];
				f32 dist     = vecArray[j].distance(vecArray[k]);
				if (dist < 35.0f) {
					sep.normalise();
					sep *= (0.5f * (35.0f - dist));
					vecArray[j] += sep;
					vecArray[k] -= sep;
				}
			}
		}
	}

	for (int i = 0; i < count; i++) {
		EnemyNode* enemyNode = new EnemyNode(enemy, spawn, 1);
		f32 dir              = JMAAtan2Radian(vecArray[i].x - spawnPos.x, vecArray[i].z - spawnPos.z);
		enemyNode->setGlobalData(vecArray[i], dir);
		tile->mEnemyNode->add(enemyNode);
	}

	mRealTypeCount[iter] += count;
	mTotalCount += count;
}

} // namespace Cave
} // namespace Game
