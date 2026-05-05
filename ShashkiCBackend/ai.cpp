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
    desk tmp = board;
    tmp.currentColor = color;
    // Если в доске установлен mustKiller, ищем продолжение рубки только если эта шашка принадлежит
    // запрошенному цвету. Это важно при рекурсивных вызовах, когда board.currentColor может отличаться.
    bool searchMustCapt = (board.mustCapt && isValidPosition(board.mustKiller) &&
        board.place[board.mustKiller.x][board.mustKiller.y].color == color);

    if (color != black && color != white) {
        color = board.currentColor;
        tmp.currentColor = color;
    }

    if (searchMustCapt) { // Если шашка продолжает рубку для этого игрока
        testStep.from.x = board.mustKiller.x;
        testStep.from.y = board.mustKiller.y;

        // Если шашка, продолжающая рубку, обычная — проверяем варианты через +2
        if (board.place[testStep.from.x][testStep.from.y].type == casual) {
            int dir[4][2] = { {-2,-2},{-2,2},{2,-2},{2,2} };
            for (int d = 0; d < 4; d++) {
                testStep.to.x = testStep.from.x + dir[d][0];
                testStep.to.y = testStep.from.y + dir[d][1];
                if (isValidPosition(testStep.to))
                    if (isCapt(tmp, testStep)) {
                        testStep.result = 2;
                        if (count < 200) var[count++] = testStep;
                    }
            }
        }
        else { // дамка — может рубить на любом расстоянии по диагонали
            int dir[4][2] = { {-1,-1},{-1,1},{1,-1},{1,1} };
            for (int d = 0; d < 4; d++) {
                int tx = testStep.from.x + dir[d][0];
                int ty = testStep.from.y + dir[d][1];
                while (tx >= 0 && tx < boardSize && ty >= 0 && ty < boardSize) {
                    testStep.to.x = tx;
                    testStep.to.y = ty;
                    if (isCapt(tmp, testStep)) {
                        testStep.result = 2;
                        if (count < 200) var[count++] = testStep;
                    }
                    tx += dir[d][0];
                    ty += dir[d][1];
                }
            }
        }
    }
    else {
        // Глобально: есть ли на доске захваты для этого цвета?
        bool anyCaptGlobal = haveAnyCapt(board, color);
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                testStep.from.x = i;
                testStep.from.y = j;
                int capt = 0; // флаг рубки для текущей фигуры
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
                            testStep.result = 2;
                            capt = 1;
                            if (count < 200) var[count++] = testStep;
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
                                // отладочный вывод состояния захвата
                                cout << testStep.from.x << " " << testStep.from.y << isCapt(tmp, testStep) << endl;
                                testStep.result = 2;
                                capt = 1;
                                if (count < 200) var[count++] = testStep;
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

                                // Добавляем обычные ходы только если на доске нет глобальных захватов
                                if (!anyCaptGlobal && isNormalMove(tmp, testStep)) {
                                    testStep.result = 1; if (count < 200) var[count++] = testStep;
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
                                // Добавляем обычный ход только если нет глобальных захватов
                                if (!anyCaptGlobal) {
                                    testStep.result = 1; if (count < 200) var[count++] = testStep;
                                }
                            }
                            else if (rv == 2) {
                                testStep.result = 2; if (count < 200) var[count++] = testStep;
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

int minimax(desk& board, int depth, bool isMaximizing, int aiColor, int diff) {

    if (depth == 0) {
        return evaluateBoard(board, aiColor);
    }
    // Определяем, чья сейчас очередь ходить на реальной доске
    bool isMax = (board.currentColor == aiColor);

    moveInfo moves[200];
    int moveCount = getAllMoves(board, moves, board.currentColor);

    if (moveCount == 0) {
        // terminal: игрок, у которого сейчас ход, не имеет ходов
        if (board.currentColor == aiColor) return -100000;
        else return 100000;
    }

    if (isMax) {
        int bestScore = -100000;
        for (int i = 0; i < moveCount; i++) {
            backupInfo backup;
            saveBackup(backup, moves[i], board);
            makeMove(board, moves[i]);
            // Определяем, кто ходит теперь (после makeMove). Это реальное значение на доске.
            bool nextIsMax = (board.currentColor == aiColor);
            int score;
            // Если хотим, чтобы противник играл 'глупо' (diff == 0) и следующий ход будет за противником,
            // не углубляемся — оцениваем позицию напрямую (симуляция тупого выбора противника).
            if (!nextIsMax && diff == 0) {
                score = evaluateBoard(board, aiColor);
            }
            else {
                score = minimax(board, depth - 1, nextIsMax, aiColor, diff);
            }
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
            bool nextIsMax = (board.currentColor == aiColor);
            int score;
            if (!nextIsMax && diff == 0) {
                score = evaluateBoard(board, aiColor);
            }
            else {
                score = minimax(board, depth - 1, nextIsMax, aiColor, diff);
            }
            loadBackup(backup, moves[i], board);
            if (score < bestScore) bestScore = score;
        }
        return bestScore;
    }
}

void getBestMove(desk& board, moveInfo& bestMove, int aiColor, int diff) {
    moveInfo moves[200];
    // Генерируем ходы для ИИ (aiColor).
    int moveCount = getAllMoves(board, moves, aiColor);

    if (moveCount == 0) {
        bestMove.result = 0;
        return;
    }

    // Выбор в зависимости от уровня сложности (diff):
    // diff == 1 (Легкий)  - выбираем первый попавшийся ход (без поиска)
    // diff == 2 (Средний) - минимакс глубиной 2
    // diff == 3 (Сложный)- минимакс глубиной 4
    if (diff <= 1) {
        // Легкий режим: мгновенный выбор первого хода
        bestMove = moves[0];
        return;
    }

    int depth = (diff == 2) ? 2 : 4;
    int bestScore = -100000;

    for (int i = 0; i < moveCount; i++) {
        backupInfo backup;
        saveBackup(backup, moves[i], board);
        makeMove(board, moves[i]);

        int score = minimax(board, depth, false, aiColor, diff);

        loadBackup(backup, moves[i], board);

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }
}
