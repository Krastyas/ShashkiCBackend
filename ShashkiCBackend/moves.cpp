#include "moves.h"
#include <cmath>
#include <iostream>
#include <windows.h>
using namespace std;



void newGame(desk* board) {
    board->black = 12;
    board->white = 12;
    board->mustCapt = 0;
    board->mustKiller.x = -1;
    board->mustKiller.y = -1;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i <= 2 and (i + j) % 2 != 0) {
                board->place[i][j].color = white;
                board->place[i][j].type = casual;
            }
            else if (i >= 5 and (i + j) % 2 != 0) {
                board->place[i][j].color = black;
                board->place[i][j].type = casual;
            }
            else {
                board->place[i][j].color = colorEmpty;
                board->place[i][j].type = typeEmpty;
            }
        }
    }
}

bool endGame(desk* board) {
    if (board->black == 0)
    {
        cout << "Белые выиграли!" << endl;
        return true;
    }

    if (board->white == 0)
    {
        cout << "Черные выиграли!" << endl;
        return true;
    }
    return false;
}


void printDesk(desk* board) {
    cout << "Ход: " << board->currentColor << endl;

    for (int i = 7; i >= 0; i--)
    {
        cout << i << " | ";

        for (int j = 0; j < 8; j++)
        {
            if (board->place[i][j].color == black and board->place[i][j].type == casual) cout << 'B' << ' ';
            else if (board->place[i][j].color == white and board->place[i][j].type == casual) cout << 'W' << ' ';
            else if (board->place[i][j].color == black and board->place[i][j].type == queen) cout << 'Q' << ' ';
            else if (board->place[i][j].color == white and board->place[i][j].type == queen) cout << 'K' << ' ';
            else cout << 'o' << ' ';
        }

        cout << endl;
    }

    cout << "   __________________" << endl;
    cout << "     0 1 2 3 4 5 6 7" << endl;
}
// =======================================================

void saveBackup(backupInfo* bk, moveInfo* step, desk* board) {
    bk->from = step->from;
    bk->to = step->to;
    bk->oldCurretnColor = board->currentColor;
    bk->oldMustCapt = board->mustCapt;

    bk->fromPiece = board->place[step->from.x][step->from.y];
    bk->toPiece = board->place[step->to.x][step->to.y];
    bk->capturedPiece = board->place[step->captured.x][step->captured.y];

    if (step->result == 2) {
        bk->oldMustKiller = board->mustKiller;
        bk->oldBLack = board->black;
        bk->oldWhite = board->white;
        bk->captured = step->captured;
    }

}

// =======================================================

bool isValidPosition(position* pos) {
    return pos->x >= 0 and pos->x <= 7 and pos->y >= 0 and pos->y <= 7;
}

bool isEmpty(desk* board, position* pos) {
    if (!isValidPosition(pos)) return false;
    return board->place[pos->x][pos->y].color == colorEmpty;
}

bool isBlackCell(position* pos) {
    if (!isValidPosition(pos)) return false;
    return (pos->x + pos->y) % 2 != 0;
}

// =======================================================

bool isNormalMove(desk* board, moveInfo* step)
{
    if (!isEmpty(board, &step->to)) return false;

    int x1 = step->from.x;
    int y1 = step->from.y;
    int x2 = step->to.x;
    int y2 = step->to.y;

    int dx = x2 - x1;
    int dy = y2 - y1;

    // дамка
    if (board->place[x1][y1].type == queen and abs(dx) == abs(dy))
    {
        int sx = (dx > 0) ? 1 : -1;
        int sy = (dy > 0) ? 1 : -1;

        position temp;
        temp.x = x1 + sx;
        temp.y = y1 + sy;

        while (temp.x != x2 or temp.y != y2)
        {
            if (!isEmpty(board, &temp)) return false;
            temp.x += sx;
            temp.y += sy;
        }

        return true;
    }

    // обычная шашка
    if (abs(dx) == 1 and abs(dy) == 1)
    {
        if (board->place[x1][y1].color == black) return dx == -1;
        if (board->place[x1][y1].color == white) return dx == 1;
    }

    return false;
}

// =======================================================

void cheat(desk* board, moveInfo* step)
{
    if (step->to.x == 14 and step->to.y == 88) {
        board->place[step->from.x][step->from.y].type = queen;
        cout << "Вы использовали чит-код!" << endl;
    }
}

// =======================================================

bool isCapt(desk* board, moveInfo* step)
{
    if (!isEmpty(board, &step->to)) return false;

    int x1 = step->from.x;
    int y1 = step->from.y;
    int x2 = step->to.x;
    int y2 = step->to.y;

    int dx = x2 - x1;
    int dy = y2 - y1;

    // обычная шашка
    if (board->place[x1][y1].type == casual)
    {
        if (abs(dx) == 2 and abs(dy) == 2)
        {
            int mx = x1 + dx / 2;
            int my = y1 + dy / 2;

            if (board->place[mx][my].color != colorEmpty and
                board->place[mx][my].color != board->place[x1][y1].color)
            {
                step->captured.x = mx;
                step->captured.y = my;
                return true;
            }
        }

        return false;
    }

    // дамка
    if (board->place[x1][y1].type == queen)
    {
        if (abs(dx) != abs(dy)) return false;

        int sx = (dx > 0) ? 1 : -1;
        int sy = (dy > 0) ? 1 : -1;

        position temp;
        temp.x = x1 + sx;
        temp.y = y1 + sy;

        bool found = false;

        while (temp.x != x2 or temp.y != y2)
        {
            if (board->place[temp.x][temp.y].color != colorEmpty)
            {
                if (board->place[temp.x][temp.y].color ==
                    board->place[x1][y1].color) return false;

                if (found) return false;

                found = true;
                step->captured = temp;
            }

            temp.x += sx;
            temp.y += sy;
        }

        return found;
    }

    return false;
}

// =======================================================

bool haveAnyCapt(desk* board, int color, int onX, int onY)
{
    moveInfo testStep;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board->place[x][y].color != color)
                continue;

            if (onX != -1 and onY != -1)
            {
                if (x != onX or y != onY)
                    continue;
            }

            testStep.from.x = x;
            testStep.from.y = y;

            if (board->place[x][y].type == casual)
            {
                int dir[4][2] = {
                    {-2,-2},{-2,2},{2,-2},{2,2}
                };

                for (int i = 0; i < 4; i++)
                {
                    testStep.to.x = x + dir[i][0];
                    testStep.to.y = y + dir[i][1];

                    if (isValidPosition(&testStep.to))
                        if (isCapt(board, &testStep))
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

                    while (tx >= 0 and tx < 8 and ty >= 0 and ty < 8)
                    {
                        testStep.to.x = tx;
                        testStep.to.y = ty;

                        if (isCapt(board, &testStep))
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

// =======================================================

int isValidMove(desk* board, moveInfo* step)
{
    if (!isValidPosition(&step->from)) return 0;
    if (!isValidPosition(&step->to)) return 0;
    if (!isEmpty(board, &step->to)) return 0;

    if (board->currentColor !=
        board->place[step->from.x][step->from.y].color) return 0;

    if (!isBlackCell(&step->to)) return 0;

    if (board->mustCapt)
    {
        if (board->mustKiller.x != step->from.x or
            board->mustKiller.y != step->from.y)
        {
            cout << "Продолжайте рубку той же шашкой!" << endl;
            return 0;
        }
    }

    if (haveAnyCapt(board, board->currentColor,
        board->mustCapt ? board->mustKiller.x : -1,
        board->mustCapt ? board->mustKiller.y : -1))
    {
        if (!isCapt(board, step))
        {
            cout << "Обязательная рубка!" << endl;
            return 0;
        }
    }

    if (isCapt(board, step)) return 2;
    if (isNormalMove(board, step)) return 1;

    return 0;
}

// =======================================================

int opposite(int color) {
    return 3 - color;
}

void makeMove(desk* board, moveInfo* step) {
        // переносим ВСЮ фигуру

        board->place[step->to.x][step->to.y] =
            board->place[step->from.x][step->from.y];

        board->place[step->from.x][step->from.y].color = colorEmpty;
        board->place[step->from.x][step->from.y].type = typeEmpty;

        // превращение
        if (step->to.x == 7 and board->place[step->to.x][step->to.y].color == white)
            board->place[step->to.x][step->to.y].type = queen;

        if (step->to.x == 0 and board->place[step->to.x][step->to.y].color == black)
            board->place[step->to.x][step->to.y].type = queen;

        // рубка
        if (step->result == 2)
        {
            if (board->place[step->captured.x][step->captured.y].color == white)
                board->white--;
            else
                board->black--;

            board->place[step->captured.x][step->captured.y].color = colorEmpty;
            board->place[step->captured.x][step->captured.y].type = typeEmpty;

            if (haveAnyCapt(board, board->currentColor, step->to.x, step->to.y))
            {
                board->mustCapt = 1;
                board->mustKiller = step->to;
                cout << "Продолжение рубки!" << endl;
                return;
            }
        }
    board->mustCapt = 0;
    board->currentColor = 3 - board->currentColor;
}
