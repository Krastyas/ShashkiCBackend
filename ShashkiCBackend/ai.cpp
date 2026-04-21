#include "moves.h"
#include "board.h"
#include <cmath>
#include <iostream>
using namespace std;


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

int getAllMoves(const desk& board) {
    moveInfo testStep = {}; 
    moveInfo var[200] = {}; int count = 0;

    if (board.mustCapt) { // Если шашка продолжает рубку
        testStep.from.x = board.mustKiller.x;
        testStep.from.y = board.mustKiller.y;
      

        int dir[4][2] = {
                   {-2,-2},{-2,2},{2,-2},{2,2}
        };

        for (int d = 0; d < 4; d++) {
            testStep.to.x = testStep.from.x + dir[d][0];
            testStep.to.y = testStep.from.y + dir[d][1];
            if (isValidPosition(testStep.to))
                if (isCapt(board, testStep)) {
                    var[count++] = testStep;
                }
        }
    }
    else {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                int x = i; int y = j;
                if (board.place[x][y].color != board.currentColor) continue;

                if (board.place[x][y].type == casual) {
                    int dir[4][2] = {
                       {-2,-2},{-2,2},{2,-2},{2,2}
                    };

                    testStep.from.x = x;
                    testStep.from.y = y;

                    for (int d = 0; d < 4; d++) {
                        testStep.to.x = testStep.from.x + dir[d][0];
                        testStep.to.y = testStep.from.y + dir[d][1];
                        if (isValidMoveForAI(board, testStep)) {
                            cout << testStep.from.x << " " << testStep.from.y << isCapt(board, testStep) << endl;
                        }
                    }
                } // Сначала мы проверяем шашки, которые идут в аттаку (казуал и квин), потом обычные шаги
            }
        }
    }
    return 1;
}
