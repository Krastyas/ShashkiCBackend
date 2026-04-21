#include "board.h"
#include <cmath>
#include <iostream>
using namespace std;

constexpr int boardSize = 8;

int evaluateBoard(const desk& board) {
    int score = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++)
        {
            if (board.currentColor == board.place[i][j].color)
            {
                if (board.place[i][j].type == casual) score += 10;
                else if (board.place[i][j].type == queen) score += 30;
            }
            else if (board.currentColor != board.place[i][j].color)
            {
                if (board.place[i][j].type == casual) score -= 10;
                else if (board.place[i][j].type == queen) score -= 30;
            }
        }
    }
    return score;
}

int getAllMoves(const desk& board, moveInfo* moves){
    moveInfo testStep;

    for (int x = 0; x < boardSize; x++)
    {
        for (int y = 0; y < boardSize; y++)
        {
            if (board.place[x][y].color != board.currentColor)
                continue;

            testStep.from.x = x;
            testStep.from.y = y;

            if (board.place[x][y].type == casual)
            {
                int dir[4][2] = {
                    {-2,-2},{-2,2},{2,-2},{2,2}
                };

                for (int i = 0; i < 4; i++)
                {
                    testStep.to.x = x + dir[i][0];
                    testStep.to.y = y + dir[i][1];

                    if (isValidPosition(testStep.to))
                        if (isCapt(const_cast<desk&>(board), testStep))
                            return true;
                }
            }
            else
            {
                int sx[4] = { -1,-1,1,1 };
                int sy[4] = { -1,1,-1,1 };

                for (int i = 0; i < 4; i++)
                {
                    int tx = x + sx[i];
                    int ty = y + sy[i];

                    while (tx >= 0 and tx < boardSize and ty >= 0 and ty < boardSize)
                    {
                        testStep.to.x = tx;
                        testStep.to.y = ty;

                        if (isCapt(board, testStep))
                            return true;

                        tx += sx[i];
                        ty += sy[i];
                    }
                }
            }
        }
    }

    return false;
}
