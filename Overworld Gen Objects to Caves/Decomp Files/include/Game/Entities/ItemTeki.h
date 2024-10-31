#ifndef _GAME_ENTITIES_ITEMTEKI_H
#define _GAME_ENTITIES_ITEMTEKI_H

#include "Game/EnemyStateMachine.h"
#include "Game/EnemyAnimatorBase.h"
#include "Game/EnemyParmsBase.h"
#include "Game/EnemyMgrBase.h"
#include "Game/EnemyBase.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "Collinfo.h"

/**
 * --Header for Overworld Gen Items--
 */

namespace Game {
namespace ItemTeki {
struct FSM;

struct Obj : public EnemyBase {
	Obj();

	//////////////// VTABLE
	virtual void onInit(CreatureInitArg* settings);
	virtual void doDirectDraw(Graphics& gfx);
	virtual void collisionCallback(CollEvent& event);
	virtual void getShadowParam(ShadowParam& settings);
	virtual ~Obj() { } 
	virtual void setInitialSetting(EnemyInitialParamBase* params);
	virtual void doUpdate();
	virtual void doDebugDraw(Graphics& gfx);
	virtual bool pressCallBack(Creature* source, f32 damage, CollPart* part);
	virtual bool hipdropCallBack(Creature* source, f32 damage, CollPart* part);
	virtual void setFSM(FSM* fsm);
	virtual EnemyTypeID::EEnemyTypeID getEnemyTypeID()
	{
		return mItemID;
	}
	//////////////// VTABLE END

	void switchID();
	void spawnDownfloor(int, u8, u8, int);
	void spawnBridge(u16);
	void spawnGate(f32, u8, bool);
	void spawnHoney(u8);
	void spawnPiki(Game::EPikiKind, int, f32, bool);
	void spawnOnyon(int, int);

	FSM* mFsm;
	EnemyTypeID::EEnemyTypeID mItemID;
};

struct Mgr : public EnemyMgrBase {
	Mgr(int objLimit, u8 modelType);

	// virtual ~Mgr();                                     // _58 (weak)
	virtual EnemyBase* birth(EnemyBirthArg&);          // _70
	virtual void createObj(int count);                 // _A0
	virtual EnemyBase* getEnemy(int idx);              // _A4
	virtual void doAlloc();                            // _A8
	virtual SysShape::Model* createModel();            // _B0
	virtual EnemyTypeID::EEnemyTypeID getEnemyTypeID() // _AC (weak)
	{
		return EnemyTypeID::EnemyID_ItemTeki;
	}

	// _00 		= VTBL
	// _00-_44	= EnemyMgrBase
	Obj* mObj; // _44, array of Objs
};

struct Parms : public EnemyParmsBase {
	struct ProperParms : public Parameters {
		inline ProperParms()
		    : Parameters(nullptr, "EnemyParmsBase")
		    , mTestParm(this, 'ip01', "�z�����݃s�L��(pom)", 5, 1, 50)
		{
		}

		Parm<int> mTestParm;           // _804, ip01
	};

	Parms() { }

	virtual void read(Stream& stream) // _08 (weak)
	{
		CreatureParms::read(stream);
		mGeneral.read(stream);
		mProperParms.read(stream);
	}

	// _00-_7F8	= EnemyParmsBase
	ProperParms mProperParms; // _7F8
};

enum AnimID {
	ITEMANIM_Wait  = 0,
	ITEMANIM_AnimCount, // 1
};

struct ProperAnimator : public EnemyAnimatorBase {
	virtual ~ProperAnimator() { }
	virtual void setAnimMgr(SysShape::AnimMgr* mgr);
	virtual SysShape::Animator& getAnimator() { return mAnimator; }
	virtual SysShape::Animator& getAnimator(int idx);

	SysShape::Animator mAnimator; 
};

/////////////////////////////////////////////////////////////////
// STATE MACHINE DEFINITIONS
enum StateID {
	ITEM_Wait  = 0,
	ITEM_Count,
};

struct FSM : public EnemyStateMachine {
	virtual void init(EnemyBase* enemy); // _08
};

struct State : public EnemyFSMState {
	inline State(u16 stateID, const char* name)
	    : EnemyFSMState(stateID)
	{
		mName = name;
	}
};

struct StateWait : public State {
	inline StateWait()
	    : State(ITEM_Wait, "wait")
	{
	}

	virtual void init(EnemyBase* enemy, StateArg* settings); // _08
	virtual void exec(EnemyBase* enemy);                     // _0C
	virtual void cleanup(EnemyBase* enemy);                  // _10
};
/////////////////////////////////////////////////////////////////
} // namespace ItemTeki
} // namespace Game

#endif
