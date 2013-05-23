#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include "menu.h"

extern Menu activeMenu;

extern MenuItem MainMenu[];
extern MenuItem SinglePlayerMenu[];
extern MenuItem MultiPlayerMenu[];
extern MenuItem PauseMenu[];

extern void showMenu();
extern void changeActiveMenu(MenuItem list[]);
extern void pauseGame();
extern void showMenu();
extern void hideMenu();

#endif//MENU_ITEM_H
