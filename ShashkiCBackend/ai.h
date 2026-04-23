#pragma once

#include "board.h"

inline int evaluateBoard(const desk& board);

int isValidMoveForAI(const desk& board, moveInfo& step);
int getAllMoves(const desk& board);