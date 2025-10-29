#include "solver.h"
#include <random>
#include <unordered_set>

std::vector<State> generateMoves(const State &s) {
  std::vector<State> moves;

  // -----------------------------
  // ATTACK MOVES
  // -----------------------------
  auto &current = s.myTurn ? s.me : s.opp;
  auto &opponent = s.myTurn ? s.opp : s.me;

  for (int i = 0; i < 2; ++i) {
    if (current[i] == 0)
      continue;

    // Skip if this hand has same value as previous hand (already processed)
    // i = 1 (for 2 hands), checkingn if my left hand is equal to my right,
    // therfore every move would be a dupe
    if (i > 0 && current[i] == current[i - 1])
      continue;

    for (int j = 0; j < 2; ++j) {
      if (opponent[j] == 0)
        continue;

      // Skip if opponent hand has same value as previous (already processed)
      if (j > 0 && opponent[j] == opponent[j - 1])
        continue;

      State next = s;
      auto &nextOpponent = next.myTurn ? next.opp : next.me;

      nextOpponent[j] += current[i];
      if (nextOpponent[j] >= 5)
        nextOpponent[j] = 0;

      next.myTurn = !next.myTurn;
      next.normalize();

      moves.push_back(next);
    }
  }

  // -----------------------------
  // SPLIT MOVES
  // -----------------------------
  int cur0 = s.myTurn ? s.me[0] : s.opp[0];
  int cur1 = s.myTurn ? s.me[1] : s.opp[1];
  int total = cur0 + cur1;

  for (int i = 0; i <= total / 2; ++i) {
    int j = total - i;
    if (i > 4 || j > 4)
      continue;
    if ((i == cur0 && j == cur1) || (i == cur1 && j == cur0))
      continue;

    State next = s;
    auto &nextCurrent = next.myTurn ? next.me : next.opp;
    nextCurrent[0] = i;
    nextCurrent[1] = j;

    next.myTurn = !next.myTurn;
    next.normalize();

    moves.push_back(next);
  }

  return moves;
}

bool isWinning(const State &s) {
  static std::unordered_map<State, bool, StateHash> memo;
  static std::unordered_set<State, StateHash> inProgress;

  auto &current = s.myTurn ? s.me : s.opp;

  if (current[0] == 0 && current[1] == 0) {
    return false;
  }

  if (memo.count(s)) {
    return memo[s];
  }

  if (inProgress.count(s)) {
    return false; // Cycle = draw = not a forced win
  }

  inProgress.insert(s);

  auto moves = generateMoves(s);
  for (const auto &move : moves) {
    if (isWinning(move) == false) {
      inProgress.erase(s);
      memo[s] = true;
      return true;
    }
  }

  inProgress.erase(s);
  memo[s] = false;
  return false;
}

State getBestMove(const State &s) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  auto moves = generateMoves(s);

  // First try to find a winning move
  for (const auto &move : moves) {
    if (!isWinning(move)) {
      return move;
    }
  }

  // No winning move - pick randomly
  if (!moves.empty()) {
    std::uniform_int_distribution<> dis(0, moves.size() - 1);
    return moves[dis(gen)];
  }

  return s; // fallback
}
