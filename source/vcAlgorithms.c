#include "settings.h"
#include "memoryManagement.h"
#include "vcAlgorithms.h"

/********************************************************************
* Function:     callAlgorithm
*--------------------------------------------------------------------
* Description:  The function callAlgorithm will extract the data of
*               the source bmp, call the algorithm given to it as
*               parameter and draw all of the share bmps, after
*               the algorithm is finished. It will use the settings
*               stored in "settings.h".
********************************************************************/
void callAlgorithm(void (*algorithm)(AlgorithmData*))
{
	int numberOfShares = NUMBER_OF_SHARES;
	Image source, *shares = xmalloc(numberOfShares*sizeof(Image));

	createSourceImage(SOURCE_PATH, &source);
    createShareFiles(DEST_PATH, shares, numberOfShares);

	/* call the algorithm */
	AlgorithmData data = {
		.source = &source,
		.shares = shares,
		.numberOfShares = numberOfShares,
	};
	algorithm(&data);

	drawShareFiles(shares, numberOfShares);
	
	/* cleanup */
	closeShareFiles(shares, numberOfShares);
	fclose(source.file);
	xfreeAll();
	fprintf(stdout, "Success!\n");
}
