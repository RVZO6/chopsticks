#include "state.h"

void State::normalize() {
  if (p1[0] > p1[1])
    std::swap(p1[0], p1[1]);
  if (p2[0] > p2[1])
    std::swap(p2[0], p2[1]);
}

bool State::operator==(const State &other) const {
  return p1 == other.p1 && p2 == other.p2 && p1Turn == other.p1Turn;
}

// makes statehash callable (functor) then define hashing. because only 5
// variables (4 hands 0-4 + turn 0-1) = 5 * 5 * 5 * 5 * 2 = 1250 possibilities
// so we basically count in mixed base (55552) here (count possibilites for
// integers 0-4 * 4, and then for 0-1)
size_t StateHash::operator()(const State &s) const noexcept {
  return s.p1[0] + s.p1[1] * 5 + s.p2[0] * 25 + s.p2[1] * 125 +
         s.p1Turn * 625;
}
