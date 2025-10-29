#include "state.h"

void State::normalize() {
  if (me[0] > me[1])
    std::swap(me[0], me[1]);
  if (opp[0] > opp[1])
    std::swap(opp[0], opp[1]);
}

bool State::operator==(const State &other) const {
  return me == other.me && opp == other.opp && myTurn == other.myTurn;
}

// makes statehash callable (functor) then define hashing. because only 5
// variables (4 hands 0-4 + turn 0-1) = 5 * 5 * 5 * 5 * 2 = 1250 possibilities
// so we basically count in mixed base (55552) here (count possibilites for
// integers 0-4 * 4, and then for 0-1)
size_t StateHash::operator()(const State &s) const noexcept {
  return s.me[0] + s.me[1] * 5 + s.opp[0] * 25 + s.opp[1] * 125 +
         s.myTurn * 625;
}
