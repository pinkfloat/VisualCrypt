#ifndef MENU_H
#define MENU_H MENU_H

/********************************************************************
* Function:     getMenu
*--------------------------------------------------------------------
* Description:  Print menu title and a list of menu items, from which
*               the user can choose one.
* Return:       Number of the menu item choosen from the user.
********************************************************************/
int getMenu (char *title, char **menuItem, int numChoices, char* prompt);

#endif
