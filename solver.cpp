#include "solver.h"
#include <random>
#include <unordered_map>
#include <unordered_set>

std::vector<State> generateMoves(const State &s) {
  std::vector<State> moves;

  // -----------------------------
  // ATTACK MOVES
  // -----------------------------
  auto &current = s.turn == Player::P1 ? s.p1 : s.p2;
  auto &opponent = s.turn == Player::P1 ? s.p2 : s.p1;

  for (int i = 0; i < 2; ++i) {
    if (current[i] == 0)
      continue;

    if (i > 0 && current[i] == current[i - 1])
      continue;

    for (int j = 0; j < 2; ++j) {
      if (opponent[j] == 0)
        continue;

      if (j > 0 && opponent[j] == opponent[j - 1])
        continue;

      State next = s;
      next.p1ChangedValues.clear();
      next.p2ChangedValues.clear();
      auto &nextOpponent = next.turn == Player::P1 ? next.p2 : next.p1;

      nextOpponent[j] += current[i];
      if (nextOpponent[j] >= 5)
        nextOpponent[j] = 0;

      if (&nextOpponent == &next.p1) {
        next.p1ChangedValues.push_back(nextOpponent[j]);
      } else {
        next.p2ChangedValues.push_back(nextOpponent[j]);
      }

      next.turn = (next.turn == Player::P1 ? Player::P2 : Player::P1);
      next.normalize();

      moves.push_back(next);
    }
  }

  // -----------------------------
  // SPLIT MOVES
  // -----------------------------
  int cur0 = s.turn == Player::P1 ? s.p1[0] : s.p2[0];
  int cur1 = s.turn == Player::P1 ? s.p1[1] : s.p2[1];
  int total = cur0 + cur1;

  for (int i = 0; i <= total / 2; ++i) {
    int j = total - i;
    if (i > 4 || j > 4)
      continue;
    if ((i == cur0 && j == cur1) || (i == cur1 && j == cur0))
      continue;

    State next = s;
    next.p1ChangedValues.clear();
    next.p2ChangedValues.clear();
    auto &nextCurrent = next.turn == Player::P1 ? next.p1 : next.p2;
    nextCurrent[0] = i;
    nextCurrent[1] = j;

    if (&nextCurrent == &next.p1) {
      next.p1ChangedValues.push_back(i);
      next.p1ChangedValues.push_back(j);
    } else {
      next.p2ChangedValues.push_back(i);
      next.p2ChangedValues.push_back(j);
    }

    next.turn = (next.turn == Player::P1 ? Player::P2 : Player::P1);
    next.normalize();

    moves.push_back(next);
  }

  return moves;
}

Outcome getOutcome(const State &s) {
  static std::unordered_map<State, Outcome, StateHash> memo;
  static std::unordered_set<State, StateHash> inProgress;

  auto &current = s.turn == Player::P1 ? s.p1 : s.p2;

  // Base case: current player has no hands
  if (current[0] == 0 && current[1] == 0) {
    return Outcome::LOSS;
  }

  // Check memo
  if (memo.count(s)) {
    return memo[s];
  }

  // Cycle detection - neither player can force a win
  if (inProgress.count(s)) {
    return Outcome::DRAW;
  }

  inProgress.insert(s);

  auto moves = generateMoves(s);

  bool hasDrawMove = false;

  for (const auto &move : moves) {
    Outcome result = getOutcome(move);

    if (result == Outcome::LOSS) {
      // Found a move that makes opponent lose - we win!
      inProgress.erase(s);
      memo[s] = Outcome::WIN;
      return Outcome::WIN;
    }

    if (result == Outcome::DRAW) {
      hasDrawMove = true;
    }
  }

  inProgress.erase(s);

  // If we have a draw move and no winning move, take the draw
  // Otherwise we lose
  Outcome outcome = hasDrawMove ? Outcome::DRAW : Outcome::LOSS;
  memo[s] = outcome;
  return outcome;
}

State getBestMove(const State &s) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  auto moves = generateMoves(s);

  // First try to find a winning move (opponent loses)
  for (const auto &move : moves) {
    if (getOutcome(move) == Outcome::LOSS) {
      return move;
    }
  }

  // Next try to find a draw move (better than losing)
  for (const auto &move : moves) {
    if (getOutcome(move) == Outcome::DRAW) {
      return move;
    }
  }

  // All moves lead to opponent winning - pick randomly
  if (!moves.empty()) {
    std::uniform_int_distribution<> dis(0, moves.size() - 1);
    return moves[dis(gen)];
  }

  return s; // fallback
}
