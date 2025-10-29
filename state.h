#pragma once
#include <algorithm>
#include <array>
#include <functional>
#include <vector>

// Game outcome from current player's perspective
enum class Outcome {
  WIN,  // Current player can force a win
  LOSS, // Current player will lose with optimal play
  DRAW  // Neither player can force a win (cycle/stalemate)
};

// Whose turn it is
enum class Player { P1, P2 };

struct State {
  std::array<int, 2> p1;
  std::array<int, 2> p2;
  Player turn;
  std::vector<int> p1ChangedValues;
  std::vector<int> p2ChangedValues;

  void normalize();
  bool operator==(const State &other) const;
};

// Hash function so we can use State as key in unordered_map
struct StateHash {
  size_t operator()(const State &s) const noexcept;
};
