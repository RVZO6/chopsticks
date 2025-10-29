#include "solver.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

// ANSI escape codes for red text
const std::string RED = "\033[31m";
const std::string RESET = "\033[0m";

std::string formatHand(int value, bool changed) {
  if (changed) {
    return RED + std::to_string(value) + RESET;
  }
  return std::to_string(value);
}

int main() {
  State startState;
  startState.p1 = {1, 1};
  startState.p2 = {1, 1};
  startState.turn = Player::P1;

  State current = startState;
  State previous = startState;
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

    previous = current;
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

    // Check what changed
    std::vector<bool> p1Changed(2, false);
    for (int val : current.p1ChangedValues) {
      for (int i = 0; i < 2; ++i) {
        if (current.p1[i] == val && !p1Changed[i]) {
          p1Changed[i] = true;
          break;
        }
      }
    }
    std::vector<bool> p2Changed(2, false);
    for (int val : current.p2ChangedValues) {
      for (int i = 0; i < 2; ++i) {
        if (current.p2[i] == val && !p2Changed[i]) {
          p2Changed[i] = true;
          break;
        }
      }
    }
    bool p1_0_changed = p1Changed[0];
    bool p1_1_changed = p1Changed[1];
    bool p2_0_changed = p2Changed[0];
    bool p2_1_changed = p2Changed[1];

    // Check outcome
    Outcome outcome = getOutcome(current);

    // Print state with move info and highlighting
    std::cout << "P1: " << formatHand(current.p1[0], p1_0_changed) << ","
              << formatHand(current.p1[1], p1_1_changed)
              << " P2: " << formatHand(current.p2[0], p2_0_changed) << ","
              << formatHand(current.p2[1], p2_1_changed)
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
