#include <stdlib.h>
#include "vcAlgorithms.h"

int main(int argc, char* argv[])
{
	int numberOfShares = 3;
	return callAlgorithm(numberOfShares, deterministicAlgorithm);
}