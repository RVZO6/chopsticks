#pragma once
#include <algorithm>
#include <array>
#include <functional> // for std::hash
#include <tuple>

struct State {
  std::array<int, 2> me;
  std::array<int, 2> opp;
  bool myTurn;

  void normalize();                          // normalize the hands
  bool operator==(const State &other) const; // equality
};

// Hash function so we can use State as key in unordered_map
struct StateHash {
  size_t operator()(const State &s) const noexcept;
};
