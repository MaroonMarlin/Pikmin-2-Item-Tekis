#include "Game/Entities/ItemTeki.h"
#include "Game/EnemyAnimKeyEvent.h"

namespace Game {
namespace ItemTeki {

void FSM::init(EnemyBase* enemy)
{
	create(ITEM_Count);
	registerState(new StateWait);
}

void StateWait::init(EnemyBase* enemy, StateArg* stateArg) { }

void StateWait::exec(EnemyBase* enemy) { }

void StateWait::cleanup(EnemyBase* enemy) { }

} // namespace ItemTeki
} // namespace Game
