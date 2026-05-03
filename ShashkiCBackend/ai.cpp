#include "moves.h"
#include "board.h"
#include <cmath>
#include <iostream>
using namespace std;

int evaluateBoard(const desk& board, int perspective) {
    int score = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (perspective == board.place[i][j].color) {
                if (board.place[i][j].type == casual) score += 10;
                else if (board.place[i][j].type == queen) score += 30;
            }
            else if (board.place[i][j].color != colorEmpty) {
                if (board.place[i][j].type == casual) score -= 10;
                else if (board.place[i][j].type == queen) score -= 30;
            }
        }
    }
    return score;
}



int getAllMoves(const desk& board, moveInfo var[200], int color) {
    moveInfo testStep = {}; 
    int count = 0;
    int capt = 0;
    desk tmp = board;
    tmp.currentColor = color;
    bool searchMustCapt = (board.mustCapt && board.currentColor == color);

    // Defensive: validate color parameter
    if (color != black && color != white) {
        color = board.currentColor;
        tmp.currentColor = color;
    }

    if (searchMustCapt) { // Если шашка продолжает рубку для этого игрока
        testStep.from.x = board.mustKiller.x;
        testStep.from.y = board.mustKiller.y;
      

        int dir[4][2] = {
                   {-2,-2},{-2,2},{2,-2},{2,2}
        };

        for (int d = 0; d < 4; d++) {
            testStep.to.x = testStep.from.x + dir[d][0];
            testStep.to.y = testStep.from.y + dir[d][1];
            if (isValidPosition(testStep.to))
                if (isCapt(tmp, testStep)) {
                    var[count++] = testStep;
                    capt = 1;
                }
        }
    }
    else {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                testStep.from.x = i;
                testStep.from.y = j;
                if (board.place[testStep.from.x][testStep.from.y].color != color) continue;
                // Сначала мы проверяем шашки, которые идут в аттаку (казуал и квин), потом обычные шаги
                if (board.place[testStep.from.x][testStep.from.y].type == casual) {

                    int dir[4][2] = {
                       {-2,-2},{-2,2},{2,-2},{2,2} // варианты ходов
                    };

     

                    for (int d = 0; d < 4; d++) {
                        testStep.to.x = testStep.from.x + dir[d][0];
                        testStep.to.y = testStep.from.y + dir[d][1];
                        if (isCapt(tmp, testStep)) {
                            capt = 1; testStep.result = 2; var[count++] = testStep;
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

                            if (isCapt(tmp, testStep)) {
                                cout << testStep.from.x << " " << testStep.from.y << isCapt(tmp, testStep) << endl;
                                capt = 1; testStep.result = 2; if (count < 200) var[count++] = testStep;
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

                                if (isNormalMove(tmp, testStep)) {
                                    testStep.result = 1; var[count++] = testStep;
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
                            int rv = isValidMove(tmp, testStep);
                            if (rv == 1) {
                                testStep.result = 1; var[count++] = testStep;
                            }
                            else if (rv == 2) {
                                testStep.result = 2; var[count++] = testStep;
                                capt = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return count;
}

int minimax(desk& board, int depth, bool isMaximizing, int aiColor) {

    if (depth == 0) {
        // Always evaluate from AI perspective, not from board.currentColor
        return evaluateBoard(board, aiColor);
    }

    moveInfo moves[200];
    int moveCount = getAllMoves(board, moves, board.currentColor);

    if (moveCount == 0) {
        if (board.currentColor == aiColor) return -100000;
        else return 100000;
    }

    if (isMaximizing) {
        int bestScore = -100000;
        for (int i = 0; i < moveCount; i++) {
            backupInfo backup;
            saveBackup(backup, moves[i], board);
            makeMove(board, moves[i]);
            int score = minimax(board, depth - 1, false, aiColor);
            loadBackup(backup, moves[i], board);
            if (score > bestScore) bestScore = score;
        }
        return bestScore;
    }
    else {
        int bestScore = 100000;
        for (int i = 0; i < moveCount; i++) {
            backupInfo backup;
            saveBackup(backup, moves[i], board);
            makeMove(board, moves[i]);
            int score = minimax(board, depth - 1, true, aiColor);
            loadBackup(backup, moves[i], board);
            if (score < bestScore) bestScore = score;
        }
        return bestScore;
    }
}

void getBestMove(desk& board, moveInfo& bestMove, int aiColor) {
    moveInfo moves[200];
    int moveCount = getAllMoves(board, moves, board.currentColor);

    if (moveCount == 0) {
        bestMove.result = 0;
        return;
    }

    int bestScore = -100000;

    for (int i = 0; i < moveCount; i++) {
        backupInfo backup;
        saveBackup(backup, moves[i], board);
        makeMove(board, moves[i]);

        int score = minimax(board, 4, false, aiColor);

        loadBackup(backup, moves[i], board);

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }
}