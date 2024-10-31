#include "Game/Entities/ItemTeki.h"

namespace Game {
namespace ItemTeki {

void ProperAnimator::setAnimMgr(SysShape::AnimMgr* mgr) { mAnimator.mAnimMgr = mgr; }

SysShape::Animator& ProperAnimator::getAnimator(int) { return mAnimator; }
} // namespace ItemTeki
} // namespace Game
