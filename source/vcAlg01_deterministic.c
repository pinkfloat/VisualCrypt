#include "booleanMatrix.h"
#include "setsNsubsets.h"
#include "vcAlgorithms.h"

void deterministicAlgorithm(Image* source, Image* share01, Image* share02)
{
    uint8_t n = 3;              /* number of shares */
    uint8_t m = 1 << (n-1);     /* number of pixels in a share per pixel in source file = 2^{n-1} */

    Set set = createSet(n, m);

    /* create basis matrices */
    BooleanMatrix B0 = createBooleanMatrix(n,m);
    BooleanMatrix B1 = createBooleanMatrix(n,m);

    uint8_t found = 0;

    for(int i = 0; i < n; i++){     /* rows */
        for(int j = 0; j < m; j++){ /* columns */

            /*___FILL_B0___*/
            /* check if i is part of the subset u_j */
            if (set.even[j].length) /* the subset contains numbers */ {
                for (int e = 0; e < set.even[j].length; e++){ /* e = element of the subset */
                    if (set.even[j].data[e] == i){
                        found = 1;
                        break;
                    }
                }
                if(found)
                    setPixel(&B0, i, j, 1);
                else
                    setPixel(&B0, i, j, 0);
                found = 0;
            }
            else /* the subset is NULL */
                setPixel(&B0, i, j, 0);

            /*___FILL_B1___*/
            /* check if i is part of the subset v_j */
            if (set.odd[j].length) /* the subset contains numbers */ {
                for (int e = 0; e < set.odd[j].length; e++){ /* e = element of the subset */
                    if (set.odd[j].data[e] == i){
                        found = 1;
                        break;
                    }
                }
                if(found)
                    setPixel(&B1, i, j, 1);
                else
                    setPixel(&B1, i, j, 0);
                found = 0;
            }
            else /* the subset is NULL */
                setPixel(&B1, i, j, 0);
        }
    }

    /* debug print */
    fprintf(stdout, "B0:\n");
    for(int i = 0; i < n; i++){     /* rows */
        for(int j = 0; j < m; j++){ /* columns */
            fprintf(stdout, "%d", getPixel(&B0, i, j));
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\nB1:\n");
    for(int i = 0; i < n; i++){     /* rows */
        for(int j = 0; j < m; j++){ /* columns */
            fprintf(stdout, "%d", getPixel(&B1, i, j));
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");

    deleteBooleanMatrix(&B0);
    deleteBooleanMatrix(&B1);
    deleteSet(&set);
}
