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
						"(k,n) random grid algorithm",
						"decrypt shares",
						"exit"
					};

	choice = getMenu("Visual Crypt Algorithms", menu, 6, "Your Choice: ");
	switch(choice)
	{
		case 1:     callAlgorithm(deterministicAlgorithm); 	break;
		case 2: 	callAlgorithm(probabilisticAlgorithm); 	break;	
		case 3: 	callAlgorithm(alternate_nn_ThresholdRGA); break;
		case 4: 	callAlgorithm(alternate_2n_ThresholdRGA); break;
		case 5:		callAlgorithm(alternate_kn_ThresholdRGA); break;
		case 6:		decryptShareFiles();					break;
		default: 	break;
	}
	return 0;
}