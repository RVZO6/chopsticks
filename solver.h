#pragma once

#include "state.h"
#include <vector>

std::vector<State> generateMoves(const State &s);
Outcome isWinning(const State &s);
State getBestMove(const State &s);
