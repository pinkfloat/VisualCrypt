#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

#define clear() fprintf(stdout, "\033[H\033[J")

/********************************************************************
* Function:     clearBuffer
*--------------------------------------------------------------------
* Description:  Clear the type-ahead buffer.
********************************************************************/
static void clearBuffer()
{
   char tmp;
   do {
     scanf("%c", &tmp);
   } while(tmp != '\n');
}

/********************************************************************
* Function:     getMenu
*--------------------------------------------------------------------
* Description:  Print menu title and a list of menu items, from which
*               the user can choose one.
* Return:       Number of the menu item choosen from the user.
********************************************************************/
int getMenu (char *title, char **menuItem, int numChoices, char* prompt)
{
   int choice = 0; /* user input value */
   int valid = 0;
   char input[3] = {'\0'};

   do {
      /* print title */
      clear();
      fprintf(stdout, "%s\n\n", title);

      /* print possible choices */
      for (int i=0; i<numChoices; i++)
      {
         fprintf(stdout, "%i. ", i+1);
         fprintf(stdout, "%s\n", *(menuItem+i));
      }
      fprintf(stdout, "\n");

      /* get user input */
      fprintf(stdout, "%s\n", prompt);
      valid = scanf("%2[0123456789]", input);
      clearBuffer();
      fprintf(stdout, "\n");

      /* repeat if the input wasn't valid */
      if (valid)
      {
         choice = atoi(input);
         if ((choice > numChoices)||(choice < 1))
            valid = 0;
      }
   } while (!valid);

   return choice;
}
