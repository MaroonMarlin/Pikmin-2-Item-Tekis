#include "Game/Entities/ItemTeki.h"
#include "Game/generalEnemyMgr.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "nans.h"

namespace Game {
namespace ItemTeki {

static const char itemtekiMgrName[]  = "ItemTekiMgr";

/**
 * @note Address: 0x80253E8C
 * @note Size: 0x50
 */
Mgr::Mgr(int objLimit, u8 modelType)
    : EnemyMgrBase(objLimit, modelType)
{
	mName = "ItemTekiManager";
}

/**
 * @note Address: 0x80253EDC
 * @note Size: 0x1F4
 */
EnemyBase* Mgr::birth(EnemyBirthArg& birthArg)
{
	return EnemyMgrBase::birth(birthArg);
}

/**
 * @note Address: 0x802540D0
 * @note Size: 0x48
 */
void Mgr::doAlloc() { init(new Parms); }

/**
 * @note Address: 0x802542E0
 * @note Size: 0x188
 */
void Mgr::createObj(int count)
{
	mObj = new Obj[count];

	EnemyTypeID::EEnemyTypeID ids[] = { EnemyTypeID::EnemyID_BridgeLong,  EnemyTypeID::EnemyID_BridgeShort,   EnemyTypeID::EnemyID_BridgeSloped,
		                                EnemyTypeID::EnemyID_Barrel, EnemyTypeID::EnemyID_ElectricGate, EnemyTypeID::EnemyID_GateLight,
                                        EnemyTypeID::EnemyID_GateHeavy, EnemyTypeID::EnemyID_RedOnyon, EnemyTypeID::EnemyID_BlueOnyon,
                                        EnemyTypeID::EnemyID_YellowOnyon, EnemyTypeID::EnemyID_DownfloorA, EnemyTypeID::EnemyID_DownfloorB,
                                        EnemyTypeID::EnemyID_DownfloorC, EnemyTypeID::EnemyID_DownfloorD, EnemyTypeID::EnemyID_PaperBag_10,
                                        EnemyTypeID::EnemyID_PaperBag_20, EnemyTypeID::EnemyID_PaperBag_40, EnemyTypeID::EnemyID_PaperBag_80,
                                        EnemyTypeID::EnemyID_PaperBag_100, EnemyTypeID::EnemyID_PaperBag_200, EnemyTypeID::EnemyID_ShipOnyon,
										EnemyTypeID::EnemyID_Pod, EnemyTypeID::EnemyID_Hole, EnemyTypeID::EnemyID_Geyser,
										EnemyTypeID::EnemyID_Nectar, EnemyTypeID::EnemyID_SpicyNectar, EnemyTypeID::EnemyID_BitterNectar,
										EnemyTypeID::EnemyID_RedPiki, EnemyTypeID::EnemyID_BluePiki, EnemyTypeID::EnemyID_YellowPiki,
										EnemyTypeID::EnemyID_WhitePiki, EnemyTypeID::EnemyID_PurplePiki, EnemyTypeID::EnemyID_ChappyPiki,
										EnemyTypeID::EnemyID_CarrotPiki, EnemyTypeID::EnemyID_RedPikiHead, EnemyTypeID::EnemyID_BluePikiHead,
										EnemyTypeID::EnemyID_YellowPikiHead, EnemyTypeID::EnemyID_WhitePikiHead, EnemyTypeID::EnemyID_PurplePikiHead,
										EnemyTypeID::EnemyID_ChappyPikiHead, EnemyTypeID::EnemyID_CarrotPikiHead};
	int id_count                    = sizeof(ids) / sizeof(EnemyTypeID::EEnemyTypeID);
	int counter                     = 0;
	for (int i = 0; i < id_count; i++) {
		int enemyNum = generalEnemyMgr->getEnemyNum(ids[i], false);
		for (int j = 0; j < enemyNum; j++, counter++) {
			mObj[counter].mItemID = ids[i];
		}
	}
}

EnemyBase* Mgr::getEnemy(int index) { return &mObj[index]; }

SysShape::Model* Mgr::createModel()
{
	SysShape::Model* model = new SysShape::Model(mModelData, J3DMODEL_ShareDL, mMtxBufferSize);
	P2ASSERTLINE(182, model != nullptr);

	for (u16 i = 0; i < mModelData->getMaterialNum(); i++) {
		const char* name = mModelData->mMaterialTable.mMaterialNames->getName(i);
		if (!strcmp(name, "hanabira1_v")) {
			model->mJ3dModel->mMatPackets[i].mShapePacket->newDifferedDisplayList(J3DMDF_DiffColorReg);
		}
	}

	model->updateModel();
	return model;
}

} // namespace ItemTeki
} // namespace Game
