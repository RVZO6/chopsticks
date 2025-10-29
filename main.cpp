#include "solver.h"
#include <iostream>

int main() {
  State startState;
  startState.p1 = {1, 1};
  startState.p2 = {1, 1};
  startState.p1Turn = true;

  State current = startState;

  // Print initial state with no move made yet
  std::cout << "P1: " << current.p1[0] << "," << current.p1[1]
             << " P2: " << current.p2[0] << "," << current.p2[1]
             << " (Initial state)\n";

  while (true) {
    auto &c = current.p1Turn ? current.p1 : current.p2;

    if (c[0] == 0 && c[1] == 0) {
      break;
    }

    current = getBestMove(current);

    // Now print with inverted turn (showing who just moved)
    std::cout << "P1: " << current.p1[0] << "," << current.p1[1]
               << " P2: " << current.p2[0] << "," << current.p2[1]
               << " Move by: " << (!current.p1Turn ? "P1" : "P2") << "\n";
  }

  std::cout << "\nWinner: " << (current.p1Turn ? "P2" : "P1") << "\n";
  return 0;
}
