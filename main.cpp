#include "solver.h"
#include <iostream>
#include <unordered_set>
#include <vector>

int main() {
  State startState;
  startState.p1 = {1, 1};
  startState.p2 = {1, 1};
  startState.turn = Player::P1;

  State current = startState;
  std::unordered_set<State, StateHash> seenStates;
  std::vector<State> history;

  // Print initial state
  std::cout << "P1: " << current.p1[0] << "," << current.p1[1]
            << " P2: " << current.p2[0] << "," << current.p2[1]
            << " (Initial state)\n";

  seenStates.insert(current);
  history.push_back(current);

  while (true) {
    auto &c = current.turn == Player::P1 ? current.p1 : current.p2;

    // Check if game is over
    if (c[0] == 0 && c[1] == 0) {
      std::cout << "\nWinner: " << (current.turn == Player::P1 ? "P2" : "P1")
                << "\n";
      break;
    }

    current = getBestMove(current);

    // Check if we've seen this state before (cycle detected)
    if (seenStates.count(current)) {
      std::cout << "\n[DRAW - Cycle detected!]\n";

      // Find where the cycle starts
      size_t cycleStart = 0;
      for (size_t i = 0; i < history.size(); ++i) {
        if (history[i] == current) {
          cycleStart = i;
          break;
        }
      }

      std::cout << "Cycle repeats these " << (history.size() - cycleStart)
                << " states:\n";
      for (size_t i = cycleStart; i < history.size(); ++i) {
        std::cout << "  P1: " << history[i].p1[0] << "," << history[i].p1[1]
                  << " P2: " << history[i].p2[0] << "," << history[i].p2[1]
                  << "\n";
      }
      break;
    }

    // Track this state
    seenStates.insert(current);
    history.push_back(current);

    // Check outcome
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
      std::cout << " [LOSING]";
    }
    std::cout << "\n";
  }

  return 0;
}
