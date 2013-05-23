#include <glCurvedAni.h>
#include "game.h"
#include "../logic/lumines.h"
#include "menu.h"
#include "menuitem.h"

enum menuid
{
    IDC_ROOT = 0,
    IDC_END = 0,
    IDC_SINGLE_PLAYER,
    IDC_MULTI_PLAYER,
    IDC_SINGLE_CHALLENGE,
    IDC_MULTI_VS_CPU,
    IDC_BACK_MAIN,
    IDC_CONTINUE_GAME,
    IDC_EXIT_GAME,
};

MenuItem MainMenu[]=
{
    // int id,	char *name, int parentid
    {IDC_SINGLE_PLAYER, "Single Player Mode", IDC_ROOT},
    {IDC_MULTI_PLAYER, "Multi-Player Mode", IDC_ROOT},
    {IDC_EXIT_GAME,     "Exit Game", IDC_ROOT},
    {IDC_END, NULL, IDC_ROOT},
};

MenuItem SinglePlayerMenu[]=
{
    // int id,	char *name, int parentid
    {IDC_SINGLE_CHALLENGE, "Challenge Mode", IDC_ROOT},
    {IDC_BACK_MAIN, "Exit to Main Menu", IDC_ROOT},
    {IDC_END, NULL, IDC_ROOT},
};

MenuItem MultiPlayerMenu[]=
{
    // int id,	char *name, int parentid
    {IDC_MULTI_VS_CPU, "Player Vs CPU", IDC_ROOT},
    {IDC_BACK_MAIN, "Exit to Main Menu", IDC_ROOT},
    {IDC_END, NULL, IDC_ROOT},
};

MenuItem PauseMenu[]=
{
    // int id,	char *name, int parentid
    {IDC_CONTINUE_GAME, "Continue Game", IDC_ROOT},
    {IDC_BACK_MAIN, "Exit to Main Menu", IDC_ROOT},
    {IDC_END, NULL, IDC_ROOT},
};

void callBack(int id);
Menu activeMenu(MainMenu, callBack);

extern Lumines *g_pGame;
void changeActiveMenu(MenuItem list[])
{
    activeMenu.changeMenu(list);
}
void pauseGame()
{
    g_pGame->pause(PR_PAUSE, true);
    changeActiveMenu(PauseMenu);
    showMenu();
}
void startGame(GameMode gamemode)
{
    hideMenu();
    g_pGame->start(gamemode);
}
void showMenu()
{
    activeMenu.setHandle(true);
    activeMenu.setVisible(true);
}
void hideMenu()
{
    activeMenu.setHandle(false);
    activeMenu.setVisible(false);
}
void callBack(int id)
{
    switch(id)
    {
    case IDC_SINGLE_PLAYER:
        changeActiveMenu(SinglePlayerMenu);
        break;
    case IDC_MULTI_PLAYER:
        changeActiveMenu(MultiPlayerMenu);
        break;
    case IDC_SINGLE_CHALLENGE:
        startGame(GM_SINGLE_CHALLENGE);
        break;
    case IDC_MULTI_VS_CPU:
        startGame(GM_MULTI_VS_CPU);
        break;
    case IDC_BACK_MAIN:
        changeActiveMenu(MainMenu);
        break;
    case IDC_CONTINUE_GAME:
        hideMenu();
        g_pGame->pause(PR_PAUSE, false);
        break;
    case IDC_EXIT_GAME:
        PostQuitMessage(0);
        break;
    }
}
