#include "state.h"
#include <iostream>
#include <vector>

std::vector<State> generateMoves(const State &s);
bool isWinning(const State &s);
State getBestMove(const State &s);

int main() {
  State startState;
  startState.me = {1, 1};
  startState.opp = {1, 1};
  startState.myTurn = true;

  State current = startState;

  // Print initial state with no move made yet
  std::cout << "Me: " << current.me[0] << "," << current.me[1]
            << " Opp: " << current.opp[0] << "," << current.opp[1]
            << " (Initial state)\n";

  while (true) {
    auto &c = current.myTurn ? current.me : current.opp;

    if (c[0] == 0 && c[1] == 0) {
      break;
    }

    current = getBestMove(current);

    // Now print with inverted turn (showing who just moved)
    std::cout << "Me: " << current.me[0] << "," << current.me[1]
              << " Opp: " << current.opp[0] << "," << current.opp[1]
              << " Move by: " << (!current.myTurn ? "Me" : "Opp") << "\n";
  }

  std::cout << "\nWinner: " << (current.myTurn ? "Opp" : "Me") << "\n";
  return 0;
}
