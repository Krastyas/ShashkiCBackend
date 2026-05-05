#pragma once

#include "board.h"

int evaluateBoard(const desk& board, int perspective);

int getAllMoves(const desk& board, moveInfo var[200], int color);

int minimax(desk& board, int depth, bool isMaximizing, int aiColor, int diff);
void getBestMove(desk& board, moveInfo& bestMove, int aiColor, int diff);