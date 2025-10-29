#include "solver.h"
#include <iostream>

int main() {
  State startState;
  startState.p1 = {1, 1};
  startState.p2 = {1, 1};
  startState.turn = Player::P1;

  State current = startState;

  // Print initial state
  std::cout << "P1: " << current.p1[0] << "," << current.p1[1]
            << " P2: " << current.p2[0] << "," << current.p2[1]
            << " (Initial state)\n";

  while (true) {
    auto &c = current.turn == Player::P1 ? current.p1 : current.p2;

    if (c[0] == 0 && c[1] == 0) {
      break;
    }

    current = getBestMove(current);

    // Check outcome after the move
    Outcome outcome = isWinning(current);

    // Print state with move info
    std::cout << "P1: " << current.p1[0] << "," << current.p1[1]
              << " P2: " << current.p2[0] << "," << current.p2[1]
              << " Move by: " << (current.turn == Player::P2 ? "P1" : "P2");

    if (outcome == Outcome::WIN) {
      std::cout << " [" << (current.turn == Player::P1 ? "P1" : "P2")
                << " WINNING]";
    } else if (outcome == Outcome::DRAW) {
      std::cout << " [DRAW]";
    } else {
      std::cout << " [LOSING POSITION]";
    }
    std::cout << "\n";
  }

  std::cout << "\nWinner: " << (current.turn == Player::P1 ? "P2" : "P1")
            << "\n";

  // Test the problematic state
  std::cout << "\n--- Debug Test ---\n";
  State testBefore;
  testBefore.p1 = {0, 2};
  testBefore.p2 = {0, 1};
  testBefore.turn = Player::P2;

  std::cout << "Test: P2's turn at {0,2} vs {0,1}\n";
  Outcome result = isWinning(testBefore);
  std::cout << "Outcome for P2: ";
  if (result == Outcome::WIN)
    std::cout << "WIN\n";
  else if (result == Outcome::LOSS)
    std::cout << "LOSS\n";
  else
    std::cout << "DRAW\n";

  auto possibleMoves = generateMoves(testBefore);
  std::cout << "P2 has " << possibleMoves.size() << " possible moves:\n";
  for (const auto &move : possibleMoves) {
    Outcome moveResult = isWinning(move);
    std::cout << "  -> P1:{" << move.p1[0] << "," << move.p1[1] << "} P2:{"
              << move.p2[0] << "," << move.p2[1] << "}"
              << " - P1 outcome: ";
    if (moveResult == Outcome::WIN)
      std::cout << "WIN\n";
    else if (moveResult == Outcome::LOSS)
      std::cout << "LOSS\n";
    else
      std::cout << "DRAW\n";
  }

  return 0;
}
