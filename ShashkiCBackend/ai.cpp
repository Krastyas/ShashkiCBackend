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

int isValidMoveForAI(const desk& board, moveInfo& step)
{
    if (!isValidPosition(step.from)) return 0;
    if (!isValidPosition(step.to)) return 0;
    if (!isEmpty(board, step.to)) return 0;

    if (board.currentColor !=
        board.place[step.from.x][step.from.y].color) return 0;

    if (!isBlackCell(step.to)) return 0;

    if (isCapt(board, step)) return 2;
    else if (isNormalMove(board, step)) return 1;

    return 0;
}

int getAllMoves(const desk& board) {
    moveInfo testStep = {}; 
    moveInfo var[200] = {}; int count = 0;
    int capt = 0;

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
                    cout << testStep.from.x << " " << testStep.from.y << isCapt(board, testStep) << endl;
                    var[count++] = testStep;
                }
        }
    }
    else {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                testStep.from.x = i;
                testStep.from.y = j;
                if (board.place[testStep.from.x][testStep.from.y].color != board.currentColor) continue;
                // Сначала мы проверяем шашки, которые идут в аттаку (казуал и квин), потом обычные шаги
                if (board.place[testStep.from.x][testStep.from.y].type == casual) {

                    int dir[4][2] = {
                       {-2,-2},{-2,2},{2,-2},{2,2} // варианты ходов
                    };

     

                    for (int d = 0; d < 4; d++) {
                        testStep.to.x = testStep.from.x + dir[d][0];
                        testStep.to.y = testStep.from.y + dir[d][1];
                        if (isCapt(board, testStep)) {
                            cout << testStep.from.x << " " << testStep.from.y << isCapt(board, testStep) << endl;
                            capt = 1; if (capt) cout << "CAPT!";
                        }
                    }
                } 
                else if (board.place[testStep.from.x][testStep.from.y].type == queen) {

                    int dir[4][2] = {
                                 {-1,-1},{-1,1},{1,-1},{1,1} // варианты ходов
                    };

                    for (int d = 0; d < 4; d++) {
                        int tx = testStep.from.x + dir[d][0];
                        int ty = testStep.from.y + dir[d][1];
                        while (tx >= 0 && tx < boardSize && ty >= 0 && ty < boardSize) {
                            testStep.to.x = tx;
                            testStep.to.y = ty;

                            if (isCapt(board, testStep)) {
                                cout << testStep.from.x << " " << testStep.from.y << isCapt(board, testStep) << endl;
                                capt = 1; if (capt) cout << "CAPT!";
                            }
          
                            tx += dir[d][0];
                            ty += dir[d][1];

                        }
                        
                    }
                }

                if (!capt) {
                    if (board.place[testStep.from.x][testStep.from.y].type == queen) {
                        int dir[4][2] = {
             {-1,-1},{-1,1},{1,-1},{1,1} // варианты ходов
                        };
                        for (int d = 0; d < 4; d++) {
                            int tx = testStep.from.x + dir[d][0];
                            int ty = testStep.from.y + dir[d][1];
                            while (tx >= 0 && tx < boardSize && ty >= 0 && ty < boardSize) {
                                testStep.to.x = tx;
                                testStep.to.y = ty;

                                if (isNormalMove(board, testStep)) {
                                    cout << testStep.from.x << " " << testStep.from.y << endl;
                                }

                                tx += dir[d][0];
                                ty += dir[d][1];

                            }
                        }
                    }
                    else if (board.place[testStep.from.x][testStep.from.y].type == casual) {
                        int dir[4][2] = {
                   {-1,-1},{-1,1},{1,-1},{1,1} // варианты ходов
                        };
                        for (int d = 0; d < 4; d++) {
                            testStep.to.x = testStep.from.x + dir[d][0];
                            testStep.to.y = testStep.from.y + dir[d][1];
                            if (isNormalMove(board, testStep)) {
                                cout << testStep.from.x << " " << testStep.from.y << endl;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
