#include <iostream>
#include <windows.h>
#include "board.h"
#include "moves.h"
#include "ai.h"

#define DEBUG 0

#if DEBUG 
    #define DBG(x) x << endl
#else
    #define DBG(x) 
    #define DBG_LN(x) 
#endif 

using namespace std;


moveInfo step;
desk board;
backupInfo backup;


int main()
{
    SetConsoleOutputCP(1251);

    newGame(board);
    cout << "Какой цвет шашек?" << endl;
    cout << "1 - черный | 2 - белый" << endl;

    cin >> board.currentColor;

    while (true)
    {
        printDesk(board);
		int score = evaluateBoard(board);
        cout << "Очки: " << score << endl;

        DBG(cout << endl << "=== ОТЛАДКА ===");
        DBG(cout << "Цвет в from (" << step.from.x << "," << step.from.y << "): " << board.place[step.from.x][step.from.y].color);
        DBG(cout << "Цвет в to (" << step.to.x << "," << step.to.y << "): " << board.place[step.to.x][step.to.y].color);
        DBG(cout << "Текущий игрок: " << board.currentColor);
        DBG(cout << "isBlackCell(to): " << isBlackCell(step.to));
        DBG(cout << "isEmpty(to): " << isEmpty(board, step.to));
        DBG(cout << "==============" << endl);

        cout << getAllMoves(board);
        cout << "Ход: X1 Y1 X2 Y2" << endl;
        cin >> step.from.x >> step.from.y >> step.to.x >> step.to.y;
        if (step.to.x == 12 && step.to.y == 0) {
            loadBackup(backup, step, board);
            continue;  

        }
        if (step.to.x == 777 && step.to.y == 0) {
            cout << "Активирован чит-код! Шашка стала королевой!" << endl;
            board.place[step.from.x][step.from.y].type = queen;
            continue;
        }

        step.result = isValidMove(board, step);

        if (!step.result) {
            cout << "Неверный ход!" << endl;
        }
            else {
                saveBackup(backup, step, board);
                makeMove(board, step);
            }
        

        if (endGame(board)) break;
    }
}