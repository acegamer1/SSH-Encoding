#include "randstate.h"
#include <stdlib.h>

gmp_randstate_t state;

void randstate_init(uint64_t seed) {
    srandom(seed); //set seed for random num generator
    gmp_randinit_mt(state); //initialize the state
    gmp_randseed_ui(state, seed); //set the seed for the state
}

void randstate_clear(void) {
    gmp_randclear(state); //clear the state
}
