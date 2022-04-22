#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

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
int getNumber(const char* prompt, int min, int max, int* result)
{
   char input[3] = {'\0'};
   fprintf(stdout, "%s\n", prompt);
   int valid = scanf("%2[0123456789]", input);
   clearBuffer();
   fprintf(stdout, "\n");

   /* check if number is between boundaries */
   if(valid)
   {
      *result = atoi(input);
      if((*result < min) || (*result > max))
         valid = 0;
   }
   return valid;
}

/********************************************************************
* Function:     getMenu
*--------------------------------------------------------------------
* Description:  Print menu title and a list of menu items, from which
*               the user can choose one.
* Return:       Number of the menu item choosen from the user.
********************************************************************/
int getMenu(const char *title, char **menuItem, int numChoices, const char* prompt)
{
   int choice = 0; /* user input value */
   int valid;

   do
   {
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
      valid = getNumber(prompt, 1, numChoices, &choice);
     
   } while (!valid);

   return choice;
}
