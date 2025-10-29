#include "state.h"

void State::normalize() {
  if (p1[0] > p1[1])
    std::swap(p1[0], p1[1]);
  if (p2[0] > p2[1])
    std::swap(p2[0], p2[1]);
}

bool State::operator==(const State &other) const {
  return p1 == other.p1 && p2 == other.p2 && turn == other.turn;
}

size_t StateHash::operator()(const State &s) const noexcept {
  return s.p1[0] + s.p1[1] * 5 + s.p2[0] * 25 + s.p2[1] * 125 +
         (s.turn == Player::P1 ? 1 : 0) * 625;
}
