#include "menu.h"
#include "vcAlgorithms.h"

int main(int argc, char* argv[])
{
	int choice, err = 0;
	char *menu [] = {
						"deterministic algorithm",
						"probabilistic algorithm",
						"random grid algorithm",
						"exit"
					};

	choice = getMenu("Encrypt Image Deluxe", menu, 4, "Your Choice: ");
	switch(choice)
	{
		case 1:     err = callAlgorithm(deterministicAlgorithm); 	break;
		case 2: 	break;	
		case 3: 	break;
		default: 	break;
	}
	return err;
}