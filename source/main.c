#include "menu.h"
#include "vcAlgorithms.h"

/********************************************************************
* Function:     main
*--------------------------------------------------------------------
* Description:  Ask the user which algorithm shall run and call the
*               chosen one.
* Return:       0 on success, -1 on failure.
********************************************************************/
int main(int argc, char* argv[])
{
	int choice;
	char *menu [] = {
						"deterministic algorithm",
						"probabilistic algorithm",
						"random grid algorithm",
						"exit"
					};
					
	choice = getMenu("Encrypt Image Deluxe", menu, 4, "Your Choice: ");
	switch(choice)
	{
		case 1:     callAlgorithm(deterministicAlgorithm); 	break;
		case 2: 	callAlgorithm(probabilisticAlgorithm); 	break;	
		case 3: 	callAlgorithm(randomGrid_nn_Threshold); break;
		default: 	break;
	}
	return 0;
}