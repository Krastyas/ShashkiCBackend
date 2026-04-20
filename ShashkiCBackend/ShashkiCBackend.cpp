#include <iostream>
#include <windows.h>
#include "board.h"
#include "moves.h"
using namespace std;
void makeBackup(backupInfo* back) {

}

moveInfo step;
desk board;


int main()
{
    SetConsoleOutputCP(1251);

    newGame(&board);
    cout << "Какой цвет шашек?" << endl;
    cout << "1 - черный | 2 - белый" << endl;

    cin >> board.currentColor;

    while (true)
    {
        printDesk(&board);

        cout << "Ход: X1 Y1 X2 Y2" << endl;
        cin >> step.from.x >> step.from.y >> step.to.x >> step.to.y;

        if (step.to.x == 14 and step.to.y == 88)
            cheat(&board, &step);

        step.result = isValidMove(&board, &step);

        if (!step.result)
            cout << "Неверный ход!" << endl;
        else
            makeMove(&board, &step);

        if (endGame(&board)) break;
    }
}