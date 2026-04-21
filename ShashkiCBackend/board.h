#pragma once

constexpr int boardSize = 8;


enum pieceType {
    typeEmpty = 0,
    casual = 1,
    queen = 2
};

enum color {
    colorEmpty = 0,
    black = 1,
    white = 2
};

struct piece {
    color color;
    pieceType type;
};

struct position {
    int x;
    int y;
};

struct moveInfo {
    position from;
    position to;
    position captured;
    int result;
};

struct desk {
    piece place[8][8] = {};
    int mustCapt = 0; // Обязательная рубка
    int currentColor = 0;
    int black = 12;
    int white = 12;
    position mustKiller = {-1, -1}; // шашка, продолжающая рубку
};

struct backupInfo {
    position from;
    position to;
    position captured;  
    position oldMustKiller;

    piece fromPiece;
    piece toPiece;        // фигуры сохраняю
    piece capturedPiece;

    int oldBLack;
    int oldWhite;
    int oldCurretnColor;
    int oldMustCapt;
    int oldResult;
  
};

