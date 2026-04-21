#pragma once

#include "board.h"

// Инициализация и отображение
void newGame(desk& board);
void printDesk(const desk& board);
bool endGame(const desk& board);

// Базовые проверки
inline bool isValidPosition(const position& pos);
inline bool isEmpty(const desk& board, const position& pos);
inline bool isBlackCell(const position& pos);

// Проверки ходов
bool isNormalMove(const desk& board, moveInfo& step);
int isValidMove(const desk& board, moveInfo& step);
void makeMove(desk& board, moveInfo& step);

// Рубка и обязательная рубка
bool haveAnyCapt(const desk& board, int color, int onX = -1, int onY = -1);
inline int opposite(int color);
	
// бэкап

// Действия
bool isCapt(desk& board, moveInfo& step);

//Бэкпап
void saveBackup(backupInfo& bk, const moveInfo& step, const desk& board);
void loadBackup(const backupInfo& bk, moveInfo& step, desk& board);

