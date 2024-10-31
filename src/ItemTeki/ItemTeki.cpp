#include "Game/Entities/ItemTeki.h"
#include "Game/EnemyFunc.h"
#include "Game/Entities/Item.h"
#include "Game/Entities/ItemGate.h"
#include "Game/Entities/ItemBridge.h"
#include "Game/BirthMgr.h"
#include "Game/gamePlayData.h"
#include "efx/TEnemyDownSmoke.h"
#include "efx/TEnemyDownWat.h"
#include "Iterator.h"
#include "Dolphin/rand.h"

namespace Game {
namespace ItemTeki {

Obj::Obj()
{
	mAnimator = new ProperAnimator;
	setFSM(new FSM);
}

void Obj::setInitialSetting(EnemyInitialParamBase*) { }

void Obj::onInit(CreatureInitArg* initArg)
{
	EnemyBase::onInit(initArg);
	disableEvent(0, EB_PlatformCollEnabled);
	disableEvent(0, EB_LeaveCarcass);
	disableEvent(0, EB_DeathEffectEnabled);
	enableEvent(0, EB_BitterImmune);
	setEmotionNone();
	mFsm->start(this, ITEM_Wait, nullptr);
	switchID();
	deathProcedure();
	kill(nullptr);
}

void Obj::doUpdate()
{
	mFsm->exec(this);
	deathProcedure();
	kill(nullptr);
}

void Obj::doDirectDraw(Graphics&) { }

void Obj::doDebugDraw(Graphics& gfx) { EnemyBase::doDebugDraw(gfx); }

void Obj::setFSM(FSM* fsm)
{
	mFsm = fsm;
	mFsm->init(this);
}

void Obj::getShadowParam(ShadowParam& shadowParam)
{
	shadowParam.mPosition.x               = mPosition.x;
	shadowParam.mPosition.y               = mPosition.y + 2.0f;
	shadowParam.mPosition.z               = mPosition.z;
	shadowParam.mBoundingSphere.mPosition = Vector3f(0.0f, 1.0f, 0.0f);
	shadowParam.mBoundingSphere.mRadius   = 0.1f;
	shadowParam.mSize                     = 0.1f;
}

bool Obj::pressCallBack(Creature* creature, f32 damage, CollPart* collpart)
{
	return false;
}

bool Obj::hipdropCallBack(Creature* creature, f32 damage, CollPart* collpart) { return false; }


void Obj::collisionCallback(CollEvent& collEvent)
{
	return;
}

/*
		SPIDERWORT SPAWN (CRASHES)

		ItemPlant::Item* PlantB = static_cast<ItemPlant::Item*>(ItemPlant::mgr->birth());
		PlantB->mPlantType = ItemPlant::PLANTTYPE_Bitter;
		PlantB->init(nullptr);
		PlantB->mFaceDir = mFaceDir;
		PlantB->setPosition(mPosition, false);
		break;

*/

void Obj::switchID() // Prepare for a GIANT switch statement
{
	EnemyTypeID::EEnemyTypeID id = getEnemyTypeID();
	switch (id) {
	case EnemyTypeID::EnemyID_BridgeSloped:
		spawnBridge(BRIDGETYPE_Sloped);
		break;
	case EnemyTypeID::EnemyID_BridgeShort:
		spawnBridge(BRIDGETYPE_Short);
		break;
	case EnemyTypeID::EnemyID_BridgeLong:
		spawnBridge(BRIDGETYPE_Long);
		break;
	case EnemyTypeID::EnemyID_Barrel:
		ItemBarrel::Item* barrel = static_cast<ItemBarrel::Item*>(ItemBarrel::mgr->birth());
		barrel->init(nullptr);
		barrel->setPosition(mPosition, false);
		break;
	case EnemyTypeID::EnemyID_Hole:
		ItemHole::Item* Hole = static_cast<ItemHole::Item*>(ItemHole::mgr->birth());
		ItemHole::InitArg HoleArg;
		HoleArg.mInitialState = ItemHole::Hole_Normal;
		Hole->init(&HoleArg);
		Hole->mFaceDir = mFaceDir;
		Hole->setPosition(mPosition, false);
		break;
	case EnemyTypeID::EnemyID_Geyser:
		ItemBigFountain::Item* Geyser = static_cast<ItemBigFountain::Item*>(ItemBigFountain::mgr->birth());
		ItemBigFountain::InitArg GeyserArg;
		GeyserArg.mInitState = ItemBigFountain::BIGFOUNTAIN_Close;
		Geyser->init(&HoleArg);
		Geyser->mFaceDir = mFaceDir;
		Geyser->setPosition(mPosition, false);
		break;
	case EnemyTypeID::EnemyID_ElectricGate:
		spawnGate(3000.0f, GATECOLOR_White, true);
		break;
	case EnemyTypeID::EnemyID_GateLight:
		spawnGate(4000.0f, GATECOLOR_White, false);
		break;
	case EnemyTypeID::EnemyID_GateHeavy:
		spawnGate(9000.0f, GATECOLOR_Black, false);
		break;
	case EnemyTypeID::EnemyID_RedOnyon:
		spawnOnyon(ONYON_OBJECT_ONYON, ONYON_TYPE_RED);
		break;
	case EnemyTypeID::EnemyID_BlueOnyon:
		spawnOnyon(ONYON_OBJECT_ONYON, ONYON_TYPE_BLUE);
		break;
	case EnemyTypeID::EnemyID_YellowOnyon:
		spawnOnyon(ONYON_OBJECT_ONYON, ONYON_TYPE_YELLOW);
		break;
	case EnemyTypeID::EnemyID_ShipOnyon:
		spawnOnyon(ONYON_OBJECT_SHIP, ONYON_TYPE_SHIP);
		break;
	case EnemyTypeID::EnemyID_Pod:
		spawnOnyon(ONYON_OBJECT_POD, ONYON_TYPE_POD);
		break;
	case EnemyTypeID::EnemyID_DownfloorA:
		spawnDownfloor('0000', DFMODEL_LargeBlock, DFTYPE_DownBlock, 0);
		break;
	case EnemyTypeID::EnemyID_DownfloorB:
		spawnDownfloor('0001', DFMODEL_LargeBlock, DFTYPE_UpBlock, 0);
		break;
	case EnemyTypeID::EnemyID_DownfloorC:
		spawnDownfloor('0002', DFMODEL_SmallBlock, DFTYPE_DownBlock, 0);
		break;
	case EnemyTypeID::EnemyID_DownfloorD:
		spawnDownfloor('0003', DFMODEL_SmallBlock, DFTYPE_UpBlock, 0);
		break;
	case EnemyTypeID::EnemyID_PaperBag_10:
		spawnDownfloor('0004', DFMODEL_PaperBag, DFTYPE_PaperBag, 10);
		break;
	case EnemyTypeID::EnemyID_PaperBag_20:
		spawnDownfloor('0005', DFMODEL_PaperBag, DFTYPE_PaperBag, 20);
		break;
	case EnemyTypeID::EnemyID_PaperBag_40:
		spawnDownfloor('0006', DFMODEL_PaperBag, DFTYPE_PaperBag, 40);
		break;
	case EnemyTypeID::EnemyID_PaperBag_80:
		spawnDownfloor('0007', DFMODEL_PaperBag, DFTYPE_PaperBag, 80);
		break;
	case EnemyTypeID::EnemyID_PaperBag_100:
		spawnDownfloor('0008', DFMODEL_PaperBag, DFTYPE_PaperBag, 100);
		break;
	case EnemyTypeID::EnemyID_PaperBag_200:
		spawnDownfloor('0009', DFMODEL_PaperBag, DFTYPE_PaperBag, 200);
		break;
	case EnemyTypeID::EnemyID_Nectar:
		spawnHoney(HONEY_Y);
		break;
	case EnemyTypeID::EnemyID_BitterNectar:
		spawnHoney(HONEY_B);
		break;
	case EnemyTypeID::EnemyID_SpicyNectar:
		spawnHoney(HONEY_R);
		break;
	case EnemyTypeID::EnemyID_RedPiki:
		spawnPiki(Red, Leaf, 0.5f, false);
		break;
	case EnemyTypeID::EnemyID_BluePiki:
		spawnPiki(Blue, Leaf, 0.5f, false);
		break;
	case EnemyTypeID::EnemyID_YellowPiki:
		spawnPiki(Yellow, Leaf, 0.5f, false);
		break;
	case EnemyTypeID::EnemyID_WhitePiki:
		spawnPiki(White, Leaf, 0.5f, false);
		break;
	case EnemyTypeID::EnemyID_PurplePiki:
		spawnPiki(Purple, Leaf, 0.5f, false);
		break;
	case EnemyTypeID::EnemyID_ChappyPiki:
		spawnPiki(Bulbmin, Leaf, 0.5f, false);
		break;
	case EnemyTypeID::EnemyID_CarrotPiki:
		spawnPiki(Carrot, Leaf, 0.5f, false);
		break;
	case EnemyTypeID::EnemyID_RedPikiHead:
		spawnPiki(Red, Flower, nullptr, true);
		break;
	case EnemyTypeID::EnemyID_BluePikiHead:
		spawnPiki(Blue, Flower, nullptr, true);
		break;
	case EnemyTypeID::EnemyID_YellowPikiHead:
		spawnPiki(Yellow, Flower, nullptr, true);
		break;
	case EnemyTypeID::EnemyID_WhitePikiHead:
		spawnPiki(White, Flower, nullptr, true);
		break;
	case EnemyTypeID::EnemyID_PurplePikiHead:
		spawnPiki(Purple, Flower, nullptr, true);
		break;
	case EnemyTypeID::EnemyID_ChappyPikiHead:
		spawnPiki(Bulbmin, Flower, nullptr, true);
		break;
	case EnemyTypeID::EnemyID_CarrotPikiHead:
		spawnPiki(Carrot, Flower, nullptr, true);
		break;
	}
}

void Obj::spawnBridge(u16 bridgetype)
{
	if (!ItemBridge::mgr) {
		ItemBridge::mgr = new ItemBridge::Mgr;
		ItemBridge::mgr->loadResources();
		itemMgr->addMgr(ItemBridge::mgr);
	}
	ItemBridge::Item* Bridge = static_cast<ItemBridge::Item*>(ItemBridge::mgr->birth());
	if (!Bridge) {
		return;
	}
	ItemBridge::BridgeInitArg* Bridgearg;
	Bridgearg->mBridgeType = bridgetype;
	Bridge->init(Bridgearg);
	Bridge->mFaceDir = mFaceDir;
	Bridge->setPosition(mPosition, false);
	return;
}

void Obj::spawnGate(f32 gatehp, u8 gatecolor, bool gateelectric)
{
	if (gateelectric && !ItemDengekiGate::mgr) {
		ItemDengekiGate::mgr = new ItemDengekiGate::Mgr;
		itemMgr->addMgr(ItemDengekiGate::mgr);
	}
	ItemGate* Gate = static_cast<ItemGate*>(itemGateMgr->birth());
	Gate->mMaxSegmentHealth     = gatehp;
	Gate->mCurrentSegmentHealth = gatehp;
	Gate->mColor                = gatecolor;
	Gate->mIsElectric 		 	= gateelectric;
	Gate->init(nullptr);
	Gate->mFaceDir = mFaceDir;
	Gate->setPosition(mPosition, false);
	return;
}

void Obj::spawnDownfloor(int dfID, u8 modeltype, u8 dftype, int weight)
{
	EnemyTypeID::EEnemyTypeID id = getEnemyTypeID();
	ItemDownFloor::Item* seesaw = static_cast<ItemDownFloor::Item*>(ItemDownFloor::mgr->birth());
	Vector3f DownfloorPos = mPosition;

	if (id == EnemyTypeID::EnemyID_DownfloorA || id == EnemyTypeID::EnemyID_DownfloorB || 
		id == EnemyTypeID::EnemyID_DownfloorC || id == EnemyTypeID::EnemyID_DownfloorD)
	{
		DownfloorPos.y += 1.0f;
	}

	seesaw->mBagMaxWeight = weight;
	seesaw->mModelType = modeltype; 
	seesaw->mDownFloorType = dftype;
	seesaw->mIsDemoBlock = true;
	seesaw->mID.setID(dfID);
	seesaw->init(nullptr);
	seesaw->mFaceDir = mFaceDir;
	seesaw->setPosition(DownfloorPos, false);
	return;
}

void Obj::spawnHoney(u8 honeytype)
{
	ItemHoney::Item* Nectar = static_cast<ItemHoney::Item*>(ItemHoney::mgr->birth());
	Nectar->init(nullptr);
	Nectar->mHoneyType = honeytype;
	Nectar->setPosition(mPosition, false);
	return;
}

void Obj::spawnPiki(Game::EPikiKind PikiType, int HappaType, f32 PluckTimer, bool Buried)
{
	ItemPikihead::Item* Piki = static_cast<ItemPikihead::Item*>(ItemPikihead::mgr->birth());
	ItemPikihead::InitArg* PikiArg;
	PikiArg->mPikminType = PikiType;
	PikiArg->mHeadType = HappaType;
	PikiArg->mAutopluckTimer = PluckTimer;
	PikiArg->mIsAlreadyBuried = Buried;
	PikiArg->mVelocity = Vector3f(0.0f);
	Piki->init(PikiArg);
	Piki->setPosition(mPosition, false);
	return;
}

void Obj::spawnOnyon(int OnyonType, int OnyonColor)
{
	if (OnyonType == ONYON_OBJECT_ONYON && (!playData->hasContainer(OnyonType)))
	{
		return; //Crashes if it tries to spawn a Onyon that is from a color you have not found. So return early in that case.
	}
	Onyon* Onyon = ItemOnyon::mgr->birth(OnyonType, OnyonColor);
	Onyon->init(nullptr);
	Onyon->mFaceDir = mFaceDir;
	Onyon->setPosition(mPosition, false);
	return;
}

} // namespace ItemTeki
} // namespace Game
