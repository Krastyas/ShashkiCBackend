#pragma once

#include "board.h"

// Инициализация и отображение
void newGame(desk* board);
void printDesk(desk* board);
bool endGame(desk* board);

// Базовые проверки
inline bool isValidPosition(position* pos);
inline bool isEmpty(desk* board, position* pos);
inline bool isBlackCell(position* pos);

// Проверки ходов
bool isNormalMove(desk* board, moveInfo* step);
bool isCapt(desk* board, moveInfo* step);
int isValidMove(desk* board, moveInfo* step);
void makeMove(desk* board, moveInfo* step);

// Рубка и обязательная рубка
bool haveAnyCapt(desk* board, int color, int onX = -1, int onY = -1);
inline int opposite(int color);
	
// бэкап
void saveBackup(backupInfo* bk, moveInfo* step, desk* board);

// Чит-код
void cheat(desk* board, moveInfo* move);