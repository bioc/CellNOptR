/* ============================================================================
 *  Name        : simulatorT1.c
 *  Author      : Aidan MacNamara
 *  Version     : 0.2
 *  revision    :	- 08.03.2018 A.Gabor: improved detection of state
 *  					 oscillations by moving average
 *  				- 20.08.2018 A.Gabor: static selection array converted to
 *  					 dynamic memory allocation (size not known apriory)
 *  Copyright
 * =========================================================================== */
#include <R.h>
#include <Rinternals.h>
#include <stdio.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

/* keep this NA > 1 and integer*/
#define NA 5
void RprintStatus(int* new_input, int nCond, int nSpecies);
void RprintMA(double* MA, int nCond, int nSpecies);



SEXP simulatorT1 (

    SEXP nStimuli_in,
    SEXP nInhibitors_in,
    SEXP nCond_in,
    SEXP nReacs_in,
    SEXP nSpecies_in,
    SEXP nSignals_in,
    SEXP nMaxInputs_in,

    SEXP finalCube_in,
    SEXP ixNeg_in,
    SEXP ignoreCube_in,
    SEXP maxIx_in,

    SEXP indexSignals_in,
    SEXP indexStimuli_in,
    SEXP indexInhibitors_in,

    SEXP valueInhibitors_in,
    SEXP valueStimuli_in,
    SEXP mode_in
) {

    SEXP simResults;

    int counter = 0;
    int i = 0;
    int j = 0;
    int s = 0;
    int a = 0, b = 0, p = 0;
    int curr_max = 0;
    int or_max = 0;
 
    int selCounter = 0;
    double *rans;

    int count_na_1 = 0;
    int count_na_2 = 0;

    /* variables */
    int nStimuli = INTEGER(nStimuli_in)[0];
    int nInhibitors = INTEGER(nInhibitors_in)[0];
    int nCond = INTEGER(nCond_in)[0];
    int nReacs = INTEGER(nReacs_in)[0];
    int nSpecies = INTEGER(nSpecies_in)[0];
    int nSignals = INTEGER(nSignals_in)[0];
    int nMaxInputs = INTEGER(nMaxInputs_in)[0];
    int mode = INTEGER(mode_in)[0];

    int count_species = 0;

    
    
    /* see stop conditions */
    float test_val = 1e-3;

    int track_cond = 0; /* track condition */
    int track_reac = 0; /* track reaction */

    int current_min;
    int dial_reac = 0;
    int dial_cond = 0;

    int term_check_1 = 1;
    float term_check_2 = 1;
    int count = 1;
    int diff;

    //Rprintf("CellNOpt simulator T1\n");
    
    counter = 0;
    int *selection;
    selection = (int*)malloc(nReacs * sizeof(int));
    for (i = 0; i < nReacs; i++) {
    	selection[i] = 0;
    }
    
    // Rprintf("checkpoint 2\n");
    
    counter = 0;
    int *maxIx;
    maxIx = (int*) malloc(nReacs * sizeof(int));
    for (i = 0; i < nReacs; i++) {
        maxIx[i] = INTEGER(maxIx_in)[counter++];
    }

    counter = 0;
    int *indexStimuli;
    indexStimuli = (int*) malloc(nStimuli * sizeof(int));
    for (i = 0; i < nStimuli; i++) {
        indexStimuli[i] = INTEGER(indexStimuli_in)[counter++];
    }

    counter = 0;
    int *indexInhibitors;
    indexInhibitors = (int*) malloc(nInhibitors * sizeof(int));
    for (i = 0; i < nInhibitors; i++) {
        indexInhibitors[i] = INTEGER(indexInhibitors_in)[counter++];
    }

    counter = 0;
    int *indexSignals;
    indexSignals = (int*) malloc(nSignals * sizeof(int));
    for (i = 0; i < nSignals; i++) {
        indexSignals[i] = INTEGER(indexSignals_in)[counter++] ;
    }

    counter = 0;
    int **finalCube;
    finalCube = (int**) malloc(nReacs * sizeof(int*));
    for (i = 0; i < nReacs; i++) {
        finalCube[i] = (int*) malloc(nMaxInputs * sizeof(int));
        for (j = 0; j < nMaxInputs; j++) {
            finalCube[i][j] = INTEGER(finalCube_in)[j * nReacs + i];
        }
    }

    counter = 0;
    int **ixNeg;
    ixNeg = (int**) malloc(nReacs * sizeof(int*));
    for (i = 0; i < nReacs; i++) {
        ixNeg[i] = (int*) malloc(nMaxInputs * sizeof(int));
        for (j = 0; j < nMaxInputs; j++) {
            ixNeg[i][j] = INTEGER(ixNeg_in)[j * nReacs + i];
        }
    }

    counter = 0;
    int **ignoreCube;
    ignoreCube = (int**) malloc(nReacs * sizeof(int*));
    for (i = 0; i < nReacs; i++) {
        ignoreCube[i] = (int*) malloc(nMaxInputs * sizeof(int));
        for (j = 0; j < nMaxInputs; j++) {
            ignoreCube[i][j] = INTEGER(ignoreCube_in)[j * nReacs + i];
        }
    }

    counter = 0;
    int **valueInhibitors;
    if (mode == 0) {
        valueInhibitors = (int**) malloc(nCond * sizeof(int*));
        for (i = 0; i < nCond; i++) {
            valueInhibitors[i] = (int*) malloc(nInhibitors * sizeof(int));
            for (j = 0; j < nInhibitors; j++) {
                valueInhibitors[i][j] = 1;
            }
        }
    }
    else {
        valueInhibitors = (int**) malloc(nCond * sizeof(int*));
        for (i = 0; i < nCond; i++) {
            valueInhibitors[i] = (int*) malloc(nInhibitors * sizeof(int));
            for (j = 0; j < nInhibitors; j++) {
                valueInhibitors[i][j] = INTEGER(valueInhibitors_in)[nCond * j + i];
            }
        }
    }

    counter = 0;
    int **valueStimuli;
    if (mode == 0) {
        valueStimuli = (int**) malloc(nCond * sizeof(int*));
        for (i = 0; i < nCond; i++) {
            valueStimuli[i] = (int*) malloc(nStimuli * sizeof(int));
            for (j = 0; j < nStimuli; j++) {
                valueStimuli[i][j] = 0;
            }
        }
    }
    else {
        valueStimuli = (int**) malloc(nCond * sizeof(int*));
        for (i = 0; i < nCond; i++) {
            valueStimuli[i] = (int*) malloc(nStimuli * sizeof(int));
            for (j = 0; j < nStimuli; j++) {
                valueStimuli[i][j] = INTEGER(valueStimuli_in)[nCond * j + i];
            }
        }
    }


    // fill end_ix - how many reactions have each species as output
    int end_ix[nSpecies];
    count_species = 0;
    for (i = 0; i < nSpecies; i++) {
        for (j = 0; j < nReacs; j++) {
            if (i == maxIx[j]) {
                count_species++;
            }
        }
        end_ix[i] = count_species;
        count_species = 0;
    }

    /* see stop conditions */
    test_val = 1e-3;

    /* create an initial values matrix*/
    int init_values[nCond][nSpecies];
    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nSpecies; j++) {
            init_values[i][j] = NA;
        }
    }

    /* set the initial values of the stimuli*/
    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nStimuli; j++) {
            init_values[i][indexStimuli[j]] = valueStimuli[i][j];
        }
    }

    /* flip and redefine inhibitors*/
    if (nInhibitors) {
        for (i = 0; i < nCond; i++) {
            for (j = 0; j < nInhibitors; j++) {
                valueInhibitors[i][j] = 1 - valueInhibitors[i][j];
                if (valueInhibitors[i][j] == 1) {
                    valueInhibitors[i][j] = NA;
                }
            }
        }
    }

    /* set the initial values of the inhibitors */
    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nInhibitors; j++) {
            init_values[i][indexInhibitors[j]] = valueInhibitors[i][j];
        }
    }

    /* initialize main loop */
    int output_prev[nCond][nSpecies];
    int new_input[nCond][nSpecies];
    double MA[nCond][nSpecies];
    double N_MA = (double)max(3,floor(0.1 * nSpecies));

    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nSpecies; j++) {
            new_input[i][j] = init_values[i][j];
            MA[i][j] = 0.5;
        }
    }
    //Rprintf("l256\n");
    //RprintMA(&MA[0][0], nCond, nSpecies);

    /*memcpy(new_input, init_values, sizeof(new_input));*/
    term_check_1 = 1;
    term_check_2 = 1;
    count = 1;


    int temp_store[nCond * nReacs][nMaxInputs];

    /* ============================================================================*/

    /* start simulation loop */
    while (term_check_1 && term_check_2) {

        /* copy to outputPrev */
        /*        memcpy(output_prev, new_input, sizeof(output_prev));*/
        for (i = 0; i < nCond; i++) {
            for (j = 0; j < nSpecies; j++) {
                output_prev[i][j] = new_input[i][j];
            }
        }
        //Rprintf("loop: %d\n", count);
        //RprintStatus(&new_input[0][0], nCond, nSpecies);

        /* fill temp store
         this is different to R version, through a single loop
         with conditions */
        track_cond = 0; /* track condition */
        track_reac = 0; /* track reaction */

        for (i = 0; i < nCond * nReacs; i++) {
            for (j = 0; j < nMaxInputs; j++) {
                /* initial values of each input */
                temp_store[i][j] = output_prev[track_cond][finalCube[track_reac][j]];

                if (ignoreCube[track_reac][j]) {
                    temp_store[i][j] = NA;
                }
                if (ixNeg[track_reac][j]) {
                    /* flip the values of the neg inputs */
                    if (temp_store[i][j] == 0) {temp_store[i][j] = 1;}
                    else if (temp_store[i][j] == 1) {temp_store[i][j] = 0;}
                }

            }

            track_cond++;
            if (track_cond == nCond) {
                track_cond = 0;
                track_reac++;
            }
        }

        /* compute the AND gates (find the min 0/1 of each row) */
        int output_cube[nCond][nReacs]; // declare output_cube
        dial_reac = 0;
        dial_cond = 0;
        for (i = 0; i < nCond * nReacs; i++) {
            current_min = temp_store[i][0];
            for (j = 1; j < nMaxInputs; j++) {
                /* if statement below is for AND gates with any NA (2) input
                 in this case output should always be NA */
                if (temp_store[i][j] == NA && ignoreCube[dial_reac][j] == 0) {
                    current_min = NA;
                    break;
                }
                else if (temp_store[i][j] < current_min) {current_min = temp_store[i][j];}
            }

            output_cube[dial_cond][dial_reac] = current_min;
            dial_cond++;
            if (dial_cond == nCond) {dial_cond = 0; dial_reac++;}
        }

        /* compute the OR gates and reinitialize new_input */
        for (i = 0; i < nCond; i++) {
            for (j = 0; j < nSpecies; j++) {
                new_input[i][j] = NA;
            }
        }

        /* declare vector to store 'selection' (R) */
        selCounter = 0;
        for (s = 0; s < nSpecies; s++) {
            /* is the species an output for any reactions?*/
            if (end_ix[s]) {

                /* find reactions with this species as output
                 add equivalent output_cube data to new_input*/
                // Rprintf("checkpoint 3\n");
                for (a = 0; a < nReacs; a++) {
                    if (s == maxIx[a]) {
                    	selection[selCounter] = a; selCounter++;}
                }
                // Rprintf("checkpoint 4\n");
                /* if the species is an output for a single reaction
                 it's a 1-1 mapping to new_input */
                if (selCounter == 1) {
                    for (b = 0; b < nCond; b++) {
                        new_input[b][s] = output_cube[b][selection[selCounter - 1]];
                    }
                    selCounter = 0;
                }
                /* else if species is output for > 1 */
                if (selCounter > 1) {
                    for (i = 0; i < nCond; i++) {
                        or_max = NA;
                        curr_max = 0;
                        for (p = 0; p < selCounter; p++) {
                            if (output_cube[i][selection[p]] >= curr_max && output_cube[i][selection[p]] < NA) {
                                or_max = output_cube[i][selection[p]];
                                curr_max = output_cube[i][selection[p]];
                            }
                        }
                        new_input[i][s] = or_max;
                    }
                    selCounter = 0;
                }
            }
        }
        // Rprintf("checkpoint 5\n");
        /* reset the stimuli */
        for (i = 0; i < nCond; i++) {
            for (j = 0; j < nStimuli; j++) {
                curr_max = valueStimuli[i][j];
                if (new_input[i][indexStimuli[j]] > curr_max && new_input[i][indexStimuli[j]] < NA) {
                    curr_max = new_input[i][indexStimuli[j]];
                }
                new_input[i][indexStimuli[j]] = curr_max;
            }
        }

        /* reset the inhibitors */
        for (i = 0; i < nCond; i++) {
            for (j = 0; j < nInhibitors; j++) {
                if (mode == 1) {
                    if (valueInhibitors[i][j] == 0) {
                        new_input[i][indexInhibitors[j]] = 0;
                    }
                }
                else {

                    /* BUG FIX Sept 2014, TC*/
                    /* at time0, inhibitors are all off by default. So, we do
                     * not want to reset their content at each tick. There is
                     * not need for the code above. This is especially important
                     * if a node if both inhibted and a readout and/or there
                     * are input links that are inhibitors*/
                }
            }
        }


        count_na_1 = 0;
        count_na_2 = 0;
        /* set 'NAs' (2s) to 0 */
        for (i = 0; i < nCond; i++) {
            for (j = 0; j < nSpecies; j++) {
                if (new_input[i][j] == NA) {
                    count_na_1++ ;
                    new_input[i][j] = 0;
                }
                if (output_prev[i][j] == NA) {
                    count_na_2++ ;
                    output_prev[i][j] = 0;
                }
            }
        }


        term_check_1 = 0;
        for (i = 0; i < nCond; i++) {
            for (j = 0; j < nSpecies; j++) {
                diff = abs((new_input[i][j] - output_prev[i][j]));
                if (diff > test_val) {
                    term_check_1 = 1;
                    break;  /*  no need to keep going checking other values if
                                one is greater than test_val */
                }
            }
//            printf("\n");
        }
        if (count_na_1 != count_na_2) {
            term_check_1 = 1;
        }
        if (count == nSpecies) {
            /* initialise moving average for each state.

            By interation nSpecies +1 all states are updated at least once, and steady states are achieved. 
            */
            for (i = 0; i < nCond; i++) {
                for (j = 0; j < nSpecies; j++) {
                    MA[i][j] = (double)new_input[i][j];
                }
            }
            //Rprintf("l 450\n");
            //RprintMA(&MA[0][0], nCond, nSpecies);
        }
        if (count > nSpecies) {
            // compute moving average for each state in each condition
            for (i = 0; i < nCond; i++) {
                for (j = 0; j < nSpecies; j++) {
                    MA[i][j] = MA[i][j] * (N_MA - 1) / N_MA + ((double)new_input[i][j]) / N_MA;
                }
            }
            //Rprintf("l 460\n");
            //RprintMA(&MA[0][0], nCond, nSpecies);
        }
        /*term_check_1 = !(abs(diff) < test_val);*/
        term_check_2 = (count < (nSpecies * 1.2));
        count++;

    }
    //Rprintf("end of while:\n");
    //RprintStatus(&new_input[0][0], nCond, nSpecies);


    /* set non-resolved bits to 2 (NA)*/
    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nSpecies; j++) {
            if (new_input[i][j] != output_prev[i][j])
                new_input[i][j] = NA;
        }
    }
    //Rprintf("set non-resolved bits to 2 (NA):\n");
    //RprintStatus(&new_input[0][0], nCond, nSpecies);
    
    /* set oscillating bits to 2 (NA)*/
    if(count > nSpecies+1){
    	for (i = 0; i < nCond; i++) {
    		for (j = 0; j < nSpecies; j++) {
    			if ((MA[i][j]<0.95) & (MA[i][j]>0.05))  // actually we could check if they are exatly 0 or 1
    				new_input[i][j] = NA;
    		}
    	}
    }
    //Rprintf("l 489\n");
    //RprintMA(&MA[0][0], nCond, nSpecies);
    //Rprintf("set oscillating bits to 2 (NA):\n");
    //RprintStatus(&new_input[0][0], nCond, nSpecies);

    PROTECT(simResults = allocMatrix(REALSXP, nCond, nSpecies));
    rans = REAL(simResults);
    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nSpecies; j++) {
            if (new_input[i][j] == NA) rans[i + nCond * j] = NA_REAL;
            else rans[i + nCond * j] = new_input[i][j];
        }
    }

    /* this code works but raise issue when calling simulatorT1 */
    /*     PROTECT(simResults = allocMatrix(REALSXP, nCond, nSignals));
        rans = REAL(simResults);
        for(i = 0; i < nCond; i++) {
            for(j = 0; j < nSignals; j++) {
                if(new_input[i][indexSignals[j]] == NA) rans[i + nCond*j] = NA_REAL;
                else rans[i + nCond*j] = new_input[i][indexSignals[j]];
            }
        }
    */


    free(maxIx);
    // Rprintf("checkpoint 6\n");
    free(selection);
    // Rprintf("checkpoint 7\n");
    free(indexStimuli);
    free(indexInhibitors);
    free(indexSignals);

    for (i = 0; i < nReacs; i++) {
        free(finalCube[i]);
    }
    free(finalCube);

    for (i = 0; i < nReacs; i++) {
        free(ixNeg[i]);
    }
    free(ixNeg);

    for (i = 0; i < nReacs; i++) {
        free(ignoreCube[i]);
    }
    free(ignoreCube);

    for (i = 0; i < nCond; i++) {
        free(valueInhibitors[i]);
    }
    free(valueInhibitors);

    for (i = 0; i < nCond; i++) {
        free(valueStimuli[i]);
    }
    free(valueStimuli);

    UNPROTECT(1);
    return simResults;

}


void RprintStatus(int* new_input, int nCond, int nSpecies) {
    int i, j;

    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nSpecies; j++) {

            Rprintf("%d ", *new_input++);
        }
        Rprintf("\n");
    }
}

void RprintMA(double* MA, int nCond, int nSpecies) {
    int i, j;

    for (i = 0; i < nCond; i++) {
        for (j = 0; j < nSpecies; j++) {

            Rprintf("%.2f ", *MA++);
        }
        Rprintf("\n");
    }
}
