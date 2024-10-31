#ifndef _GAME_ENEMYINFO_H
#define _GAME_ENEMYINFO_H

#include "types.h"

namespace Game {

extern int gEnemyInfoNum;

struct EnemyInfo {
	char* mName;        // _00
	char mId;           // _04
	char mParentID;     // _05
	char mMembers;      // _06
	u16 mFlags;         // _08
	char* mModelName;   // _0C
	char* mAnimName;    // _10
	char* mAnimMgrName; // _14
	char* mTextureName; // _18
	char* mParamName;   // _1C
	char* mCollName;    // _20
	char* mStoneName;   // _24
	int mChildID;       // _28
	int mChildNum;      // _2C
	char mBitterDrops;  // _30
};

enum EnemyInfoFlags {
	// Note: 2 is enabled anywhere EFlag_CanBeSpawned is, but doesnt seem to do anything

	// 0x100 is also used for a few (dwarf bulborbs and sheargrubs), but doesnt seem to have a purpose
	// It may have originally been another day end take off max count flag, but max 7 is just the default instead

	EFlag_UseOwnID        = 1,     // Should this enemy use its own ID instead of the parent ID
	EFlag_CanBeSpawned    = 4,     // Can be spawned at all, should be true unless youre UmiMushiBase or Pom (seems to only apply to caves)
	EFlag_CanAppearDayEnd = 0x10,  // Can this enemy appear in the day end takeoff at all
	EFlag_DayEndMax1      = 0x20,  // Max 1 of these enemies in day end takeoff
	EFlag_DayEndMax2      = 0x40,  // Max 2 of these enemies in day end takeoff (no enemy seems to use this one in particular)
	EFlag_DayEndMax4      = 0x80,  // Max 4 of these enemies (If none of the max count flags are set, 7 is the max)
	EFlag_HasNoInfo       = 0x200, // Don't track pikmin lost/creatures defeated/piklopedia entered
};

enum EBitterDropType { // ID
	BDT_Weak      = 0,
	BDT_Normal    = 1,
	BDT_Strong    = 2,
	BDT_Triple    = 3,
	BDT_Empty     = 4,
	BDT_EmptyTwo  = 5,
	BDT_MiniBoss  = 6,
	BDT_Boss      = 7,
	BDT_FinalBoss = 8,
};

// clang-format off
struct EnemyTypeID {
enum EEnemyTypeID {       // ID      Common Name
	// If a Wild Pikmin is spawned of a type that has not been discovered yet, the pikmin discovery cutscene will play upon floor entry.
	// If an onion is set to be spawned of a type that has not been discovered yet, it will not spawn.
	// Onion code would need to be changed for onions to function in challenge mode.
	// Multiple ship pods, onions, and Hocotate Ships can cause missing particles.
	// Downfloors need at least 2 members of the same ID to load, otherwise they will crash. (Not counting Paper Bags)
	// Any Downfloor past the second of the same ID will not function properly. (Not counting Paper Bags)
	EnemyID_CarrotPikiHead = -43, //Buried Carrot Flower
	EnemyID_ChappyPikiHead = -42, //Buried Bulbmin Flower
	EnemyID_PurplePikiHead = -41, //Buried Purple Flower Pikmin
	EnemyID_WhitePikiHead  = -40, //Buried White Flower Pikmin
	EnemyID_YellowPikiHead = -39, //Buried Yellow Flower Pikmin
	EnemyID_BluePikiHead   = -38, //Buried Blue Flower Pikmin
	EnemyID_RedPikiHead    = -37, //Buried Red Flower Pikmin
	EnemyID_CarrotPiki     = -36, //Wild Carrot Leaf
	EnemyID_ChappyPiki     = -35, //Wild Bulbmin Leaf
	EnemyID_PurplePiki     = -34, //Wild Purple Leaf Pikmin
	EnemyID_WhitePiki	   = -33, //Wild White Leaf Pikmin
	EnemyID_YellowPiki	   = -32, //Wild Yellow Leaf Pikmin
	EnemyID_BluePiki	   = -31, //Wild Blue Leaf Pikmin
	EnemyID_RedPiki		   = -30, //Wild Red Leaf Pikmin
	EnemyID_YellowOnyon    = -29, //Yellow Onion
	EnemyID_BlueOnyon      = -28, //Blue Onion
	EnemyID_RedOnyon       = -27, //Red Onion
	EnemyID_ShipOnyon      = -26, //Hocotate Ship
	EnemyID_Pod            = -25, //Cave Ship Pod
	EnemyID_Geyser         = -24, //Closed Geyser
	EnemyID_Hole		   = -23, //Cave Hole, Crashes when entered with no proceeding floor.
	EnemyID_PaperBag_200   = -22, //ID '0009', Paper Bag with 200 weight tolerance
	EnemyID_PaperBag_100   = -21, //ID '0008', Paper Bag with 100 weight tolerance
	EnemyID_PaperBag_80    = -20, //ID '0007', Paper Bag with 80 weight tolerance
	EnemyID_PaperBag_40    = -19, //ID '0006', Paper Bag with 40 weight tolerance
	EnemyID_PaperBag_20    = -18, //ID '0005', Paper Bag with 20 weight tolerance
	EnemyID_PaperBag_10    = -17, //ID '0004', Paper Bag with 10 weight tolerance
	EnemyID_DownfloorD     = -16, //ID '0003', Small Downfloor Block. First load starts up.
	EnemyID_DownfloorC     = -15, //ID '0002', Small Downfloor Block. First load starts down.
	EnemyID_DownfloorB     = -14, //ID '0001', Large Downfloor Block. First load starts up.
	EnemyID_DownfloorA     = -13, //ID '0000', Large Downfloor Block. First load starts down. 
	EnemyID_SpicyNectar    = -12, //Bitter Spray Drop
	EnemyID_BitterNectar   = -11, //Spicy Spray Drop
	EnemyID_Nectar         = -10, //Nectar Drop
	EnemyID_GateHeavy      = -9,  //Black Gate with 9000HP
	EnemyID_GateLight      = -8,  //White Gate with 4000HP
	EnemyID_ElectricGate   = -7,  //Electric Gate with 3000HP
	EnemyID_Barrel         = -6,  //Hole Clog
	EnemyID_BridgeLong     = -5,  //Long Bridge Type
	EnemyID_BridgeShort    = -4,  //Short Bridge Type
	EnemyID_BridgeSloped   = -3,  //Sloped Bridge Type
	EnemyID_ItemTeki       = -2,  //Parent ID, crashes if loaded
	EnemyID_NULL           = -1,  // ID not set
	EnemyID_Pelplant       = 0,	  // Pellet Posy
	EnemyID_Kochappy       = 1,	  // Dwarf Red Bulborb
	EnemyID_Chappy         = 2,	  // Red Bulborb
	EnemyID_BluePom        = 3,	  // Lapis Lazuli Candypop Bud
	EnemyID_RedPom         = 4,	  // Crimson Candypop Bud
	EnemyID_YellowPom      = 5,	  // Golden Candypop Bud
	EnemyID_BlackPom       = 6,	  // Violet Candypop Bud
	EnemyID_WhitePom       = 7,	  // Ivory Candypop Bud
	EnemyID_RandPom        = 8,	  // Queen Candypop Bud
	EnemyID_Kogane         = 9,	  // Iridescent Flint Beetle
	EnemyID_Wealthy        = 10,  // Iridescent Glint Beetle
	EnemyID_Fart           = 11,  // Doodlebug
	EnemyID_UjiA           = 12,  // Female Sheargrub
	EnemyID_UjiB           = 13,  // Male Sheargrub
	EnemyID_Tobi           = 14,  // Shearwig
	EnemyID_Armor          = 15,  // Cloaking Burrow-nit
	EnemyID_Qurione        = 16,  // Honeywisp
	EnemyID_Frog           = 17,  // Yellow Wollywog
	EnemyID_MaroFrog       = 18,  // Wollywog
	EnemyID_Rock           = 19,  // Falling boulder
	EnemyID_Hiba           = 20,  // Fire geyser
	EnemyID_GasHiba        = 21,  // Gas pipe
	EnemyID_ElecHiba       = 22,  // Electrical wire
	EnemyID_Sarai          = 23,  // Swooping Snitchbug
	EnemyID_Tank           = 24,  // Fiery Blowhog
	EnemyID_Wtank          = 25,  // Watery Blowhog
	EnemyID_Catfish        = 26,  // Water Dumple
	EnemyID_Tadpole        = 27,  // Wogpole
	EnemyID_ElecBug        = 28,  // Anode Beetle
	EnemyID_Mar            = 29,  // Puffy Blowhog
	EnemyID_Queen          = 30,  // Empress Bulblax
	EnemyID_Baby           = 31,  // Bulborb Larva
	EnemyID_Demon          = 32,  // Bumbling Snitchbug
	EnemyID_FireChappy     = 33,  // Fiery Bulblax
	EnemyID_SnakeCrow      = 34,  // Burrowing Snagret
	EnemyID_KumaChappy     = 35,  // Spotty Bulbear
	EnemyID_Bomb           = 36,  // Bomb-rock
	EnemyID_Egg            = 37,  // Egg
	EnemyID_PanModoki      = 38,  // Breadbug
	EnemyID_PanModokiNest  = 39,  // Breadbug Nest
	EnemyID_OoPanModoki    = 40,  // Giant Breadbug
	EnemyID_Fuefuki        = 41,  // Antenna Beetle
	EnemyID_BlueChappy     = 42,  // Orange Bulborb
	EnemyID_YellowChappy   = 43,  // Hairy Bulborb
	EnemyID_BlueKochappy   = 44,  // Dwarf Orange Bulborb
	EnemyID_YellowKochappy = 45,  // Snow Bulborb
	EnemyID_Tanpopo        = 46,  // Dandelion
	EnemyID_Clover         = 47,  // Clover
	EnemyID_HikariKinoko   = 48,  // Common Glowcap
	EnemyID_Ooinu_s        = 49,  // Figwort (red small)
	EnemyID_Ooinu_l        = 50,  // Figwort (red large)
	EnemyID_Wakame_s       = 51,  // Shoot (small)
	EnemyID_Wakame_l       = 52,  // Shoot (large)
	EnemyID_KingChappy     = 53,  // Emperor Bulblax
	EnemyID_Miulin         = 54,  // Mamuta
	EnemyID_Hanachirashi   = 55,  // Withering Blowhog
	EnemyID_Damagumo       = 56,  // Beady Long Legs
	EnemyID_Kurage         = 57,  // Lesser Spotted Jellyfloat
	EnemyID_BombSarai      = 58,  // Careening Dirigibug
	EnemyID_FireOtakara    = 59,  // Fiery Dweevil
	EnemyID_WaterOtakara   = 60,  // Caustic Dweevil
	EnemyID_GasOtakara     = 61,  // Munge Dweevil
	EnemyID_ElecOtakara    = 62,  // Anode Dweevil
	EnemyID_Jigumo         = 63,  // Hermit Crawmad
	EnemyID_JigumoNest     = 64,  // Hermit Crawmad Nest
	EnemyID_Imomushi       = 65,  // Ravenous Whiskerpillar
	EnemyID_Houdai         = 66,  // Man-at-Legs
	EnemyID_LeafChappy     = 67,  // Bulbmin
	EnemyID_TamagoMushi    = 68,  // Mitite
	EnemyID_BigFoot        = 69,  // Raging Long Legs
	EnemyID_SnakeWhole     = 70,  // Pileated Snagret
	EnemyID_UmiMushi       = 71,  // Ranging Bloyster
	EnemyID_OniKurage      = 72,  // Greater Spotted Jellyfloat
	EnemyID_BigTreasure    = 73,  // Titan Dweevil
	EnemyID_Stone          = 74,  // Rock (projectile)
	EnemyID_Kabuto         = 75,  // Armored Cannon Beetle Larva
	EnemyID_KumaKochappy   = 76,  // Dwarf Bulbear
	EnemyID_ShijimiChou    = 77,  // Unmarked Spectralids
	EnemyID_MiniHoudai     = 78,  // Gatling Groink
	EnemyID_Sokkuri        = 79,  // Skitter Leaf
	EnemyID_Tukushi        = 80,  // Horsetail
	EnemyID_Watage         = 81,  // Seeding Dandelion
	EnemyID_Pom            = 82,  // Candypop Bud base (crashes)
	EnemyID_PanHouse       = 83,  // Breadbug Nest
	EnemyID_Hana           = 84,  // Creeping Chrysanthemum
	EnemyID_DaiodoRed      = 85,  // Glowstem (red)
	EnemyID_DaiodoGreen    = 86,  // Glowstem (green)
	EnemyID_Magaret        = 87,  // Margaret
	EnemyID_Nekojarashi    = 88,  // Foxtail
	EnemyID_Chiyogami      = 89,  // Chigoyami paper
	EnemyID_Zenmai         = 90,  // Fiddlehead
	EnemyID_KareOoinu_s    = 91,  // Figwort (brown small)
	EnemyID_KareOoinu_l    = 92,  // Figwort (brown large)
	EnemyID_BombOtakara    = 93,  // Volatile Dweevil
	EnemyID_DangoMushi     = 94,  // Segmented Crawbster
	EnemyID_Rkabuto        = 95,  // Decorated Cannon Beetle
	EnemyID_Fkabuto        = 96,  // Armored Cannon Beetle Larva (burrowed)
	EnemyID_FminiHoudai    = 97,  // Gatling Groink (pedestal)
	EnemyID_Tyre           = 98,  // Waterwraith rollers
	EnemyID_BlackMan       = 99,  // Waterwraith
	EnemyID_UmiMushiBase   = 100, // Bloyster base (crashes)
	EnemyID_UmiMushiBlind  = 101, // Toady Bloyster
	EnemyID_COUNT,
};
EEnemyTypeID mEnemyID; // _00
u8 mCount;             // _04
};
// clang-format on

extern EnemyInfo gEnemyInfo[];

struct EnemyNumInfo {
	EnemyNumInfo()
	    : mEnemyNumList(nullptr)
	{
	}

	static int getOriginalEnemyID(int enemyID);

	void init();
	void resetEnemyNum();
	void addEnemyNum(int enemyID, u8 num);
	u8 getEnemyNum(int enemyID, bool doCheckOriginal);
	u8 getEnemyNumData(int enemyID);

	u8 _00[4];                  // _00
	EnemyTypeID* mEnemyNumList; // _04
};

namespace EnemyInfoFunc {
EnemyInfo* getEnemyInfo(int enemyID, int flags);
char* getEnemyName(int enemyID, int flags);
char* getEnemyResName(int enemyID, int flags);
char getEnemyMember(int enemyID, int flags);
int getEnemyID(char* name, int flags);
} // namespace EnemyInfoFunc

inline int getEnemyMgrID(int enemyID)
{
	int idx = -1;

	for (int i = 0; i < gEnemyInfoNum; i++) {
		char id = gEnemyInfo[i].mId;

		if (id == enemyID) {
			idx = (gEnemyInfo[i].mFlags & EFlag_UseOwnID) ? enemyID : gEnemyInfo[i].mParentID;
		}
	}

	return idx;
}

#define SHIJIMICHOU_GROUP_COUNT 25
#define TAMAGOMUSHI_GROUP_COUNT 30

#define IS_ENEMY_BOSS(id)                                                                                                        \
	(id == EnemyTypeID::EnemyID_Queen || id == EnemyTypeID::EnemyID_SnakeCrow || id == EnemyTypeID::EnemyID_KingChappy           \
	 || id == EnemyTypeID::EnemyID_Damagumo || id == EnemyTypeID::EnemyID_OoPanModoki || id == EnemyTypeID::EnemyID_Houdai       \
	 || id == EnemyTypeID::EnemyID_UmiMushiBlind || id == EnemyTypeID::EnemyID_BlackMan || id == EnemyTypeID::EnemyID_DangoMushi \
	 || id == EnemyTypeID::EnemyID_BigFoot || id == EnemyTypeID::EnemyID_SnakeWhole || id == EnemyTypeID::EnemyID_UmiMushi       \
	 || id == EnemyTypeID::EnemyID_BigTreasure)

} // namespace Game
#endif
