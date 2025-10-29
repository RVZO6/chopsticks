#pragma once

#include "state.h"
#include <vector>

std::vector<State> generateMoves(const State &s);
Outcome getOutcome(const State &s);
State getBestMove(const State &s);
