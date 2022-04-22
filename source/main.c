#include "decrypt.h"
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
						"(n,n) deterministic algorithm",
						"(n,n) probabilistic algorithm",
						"(n,n) random grid algorithm",
						"(2,n) random grid algorithm",
						"decrypt shares",
						"exit"
					};

	choice = getMenu("Encrypt Image Deluxe", menu, 5, "Your Choice: ");
	switch(choice)
	{
		case 1:     callAlgorithm(deterministicAlgorithm); 	break;
		case 2: 	callAlgorithm(probabilisticAlgorithm); 	break;	
		case 3: 	callAlgorithm(randomGrid_nn_Threshold); break;
		case 4: 	callAlgorithm(randomGrid_2n_Threshold); break;
		case 5:		decryptShareFiles();					break;
		default: 	break;
	}
	return 0;
}