#include "vcAlgorithms.h"

#include "fileManagement.h"
#include "memoryManagement.h"
#include "menu.h"
#include "settings.h"

void mallocSharesOfSourceSize(Image *source, Image *share, int numberOfShares) {
    // for each share
    for (int i = 0; i < numberOfShares; i++) {
        share[i].height = source->height;
        share[i].width = source->width;
        mallocPixelArray(&share[i]);
    }
}

void callAlgorithm(void (*algorithm)(AlgorithmData *), int algorithmNumber) {
    int numberOfShares = getNfromUser();

    Image source, *shares = xmalloc(numberOfShares * sizeof(Image));

    createSourceImage(&source);
    deleteShareFiles();
    createShareFiles(shares, numberOfShares);

    FILE *randomSrc = xfopen(RANDOM_FILE_PATH, "r");

    AlgorithmData data = {.source = &source,
                          .shares = shares,
                          .numberOfShares = numberOfShares,
                          .algorithmNumber = RG_VERSION ? algorithmNumber + 3 : algorithmNumber,
                          .randomSrc = randomSrc};
    algorithm(&data);

    drawShareFiles(shares, numberOfShares);

    xcloseAll();
    xfreeAll();
    fprintf(stdout, "Success!\n");
}
