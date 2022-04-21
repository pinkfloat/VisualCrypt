#include "settings.h"
#include "memoryManagement.h"
#include "fileManagement.h"
#include "vcAlgorithms.h"

/********************************************************************
* Function:     mallocSharesOfSourceSize
*--------------------------------------------------------------------
* Description:  This function will allocate the buffer of the share
*               pixel arrays, that are going to be printed to the
*               BMPs later. The size will be the same as for the
*				source file.
* Input:        source = containing width and height of the source
*               image,
*               numberOfShares = amount of shares that will be
*               created,
* Output:       share->array will be correctly allocated for each
*               share.
********************************************************************/
void mallocSharesOfSourceSize(Image* source, Image* share, int numberOfShares)
{
    /* for each share */
    for(int i = 0; i < numberOfShares; i++)
    {
        share[i].height = source->height;
        share[i].width = source->width;
        mallocPixelArray(&share[i]);
    }
}

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
	deleteShareFiles(DEST_PATH);
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
	xcloseAll();
	xfreeAll();
	fprintf(stdout, "Success!\n");
}
