#include "decrypt.h"
#include "menu.h"
#include "vcAlgorithms.h"
#include "vcAlg01_deterministic.h"
#include "vcAlg02_probabilistic.h"
#include "vcAlg03_randomGrid.h"

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
						"(k,n) random grid algorithm",
						"decrypt shares",
						"exit"
					};

	choice = getMenu("Visual Crypt Algorithms", menu, 6, "Your Choice: ");
	switch(choice)
	{
		case 1:     callAlgorithm(deterministicAlgorithm, 0); 	break;
		case 2: 	callAlgorithm(probabilisticAlgorithm, 0); 	break;	
		case 3: 	callAlgorithm(callRandomGridAlgorithm, 1); break;
		case 4: 	callAlgorithm(callRandomGridAlgorithm, 2); break;
		case 5:		callAlgorithm(callRandomGridAlgorithm, 3); break;
		case 6:		decryptShareFiles();					break;
		default: 	break;
	}
	return 0;
}