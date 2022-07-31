#ifndef MENU_H
#define MENU_H MENU_H

#define clear() fprintf(stdout, "\033[H\033[J")

/********************************************************************
 * Function:     getNumber
 *--------------------------------------------------------------------
 * Description:  Prints prompt and get a maximum of two numbers from
 *               user, which are stored in result. If the input
 *               was a valid number, it will return the number of
 *               successfully read characters.
 * Input:        prompt = printed before user input is taken,
 *               min = minimal valid value
 *               max = maximal valid value
 * Output:       result = number read from user input
 * Return:       number of successfully read characters on success,
 *               EOF on failure.
 ********************************************************************/
int getNumber(const char *prompt, int min, int max, int *result);

/********************************************************************
 * Function:     getNfromUser
 *--------------------------------------------------------------------
 * Description:  Ask the user the number of shares that should be
 *               generated.
 ********************************************************************/
int getNfromUser();

/********************************************************************
 * Function:     getKfromUser
 *--------------------------------------------------------------------
 * Description:  Ask the user the number of the k shares in a
 *               (k,n) RG-Algorithm.
 ********************************************************************/
int getKfromUser(int n);

/********************************************************************
 * Function:     getMenu
 *--------------------------------------------------------------------
 * Description:  Print menu title and a list of menu items, from which
 *               the user can choose one.
 * Return:       Number of the menu item choosen from the user.
 ********************************************************************/
int getMenu(const char *title, char **menuItem, int numChoices, const char *prompt);

#endif
