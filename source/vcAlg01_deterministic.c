#include <stdlib.h>
#include "vcAlgorithms.h"

void deterministicAlgorithm(Image* source, Image* share01, Image* share02)
{
    uint8_t n = 2;              /* number of shares */
    uint8_t m = 1 << (n-1);     /* number of pixels in a share per pixel in source file = 2^{n-1} */

    /* create basis set "w" */
    int w[n];
    for(uint8_t i = 0; i < n; i++)
        w[i] = i;

    /* will be replaced by auto-calculation of the subsets */
        /* u = even cardinality subsets of w */
        uint8_t u_0[] = {0, 1};
        uint8_t* u_1 = NULL;
        uint8_t* u[] = {u_0, u_1};
        uint8_t u_len = 2;

        /* v = odd cardinality subsets of w */
        uint8_t v_0[] = {0};
        uint8_t v_1[] = {1};
        uint8_t* v[] = {v_0, v_1};
        uint8_t v_len = 1;

    /* create basis matrices */
    int B0[n][m];
    int B1[n][m];

    uint8_t found = 0;

    for(int i = 0; i < n; i++){     /* rows */
        for(int j = 0; j < m; j++){ /* columns */

            /*___FILL_B0___*/
            /* check if i is part of the subset u_j */
            if (u[j]) /* the subset contains numbers */ {
                for (int e = 0; e < u_len; e++){ /* e = element of the subset */
                    if (u[j][e] == i){
                        found = 1;
                        break;
                    }
                }
                if(found)
                    B0[i][j] = 1;
                else
                    B0[i][j] = 0;
                found = 0;
            }
            else /* the subset is NULL */
                B0[i][j] = 0;

            /*___FILL_B1___*/
            /* check if i is part of the subset v_j */
            if (v[j]) /* the subset contains numbers */ {
                for (int e = 0; e < v_len; e++){ /* e = element of the subset */
                    if (v[j][e] == i){
                        found = 1;
                        break;
                    }
                }
                if(found)
                    B1[i][j] = 1;
                else
                    B1[i][j] = 0;
                found = 0;
            }
            else /* the subset is NULL */
                B1[i][j] = 0;
        }
    }

    /* debug print */
    fprintf(stdout, "B0:\n");
    for(int i = 0; i < n; i++){     /* rows */
        for(int j = 0; j < m; j++){ /* columns */
            fprintf(stdout, "%d", B0[i][j]);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\nB1:\n");
    for(int i = 0; i < n; i++){     /* rows */
        for(int j = 0; j < m; j++){ /* columns */
            fprintf(stdout, "%d", B1[i][j]);
        }
        fprintf(stdout, "\n");
    }
}
