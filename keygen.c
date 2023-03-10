#include "ss.c"
#include "numtheory.c"
#include "randstate.c"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define OPTIONS "b:i:n:d:s:vh"

uint64_t bits = 256;
uint64_t iterations = 50;
uint64_t verbose = 0;

char pbfile[100] = "ss.pub";
char pvfile[100] = "ss.priv";

int main(int argc, char **argv) {
    uint64_t seed = time(NULL);
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b':
            bits = atoi(optarg); //set bits
            break;
        case 'i': iterations = atoi(optarg); break;
        case 'n': strcpy(pbfile, optarg); break;
        case 'd': strcpy(pvfile, optarg); break;
        case 's': seed = atoi(optarg); break;
        case 'v':
            ++verbose; //set verbose to true
            break;
        case 'h':
            printf("Available command line options\n\
• -b : specifies the minimum bits needed for the public modulus n.\n\
• -i : specifies the number of Miller-Rabin iterations for testing primes (default: 50).\n\
• -n pbfile : specifies the public key file (default: ss.pub).\n\
• -d pvfile : specifies the private key file (default: ss.priv).\n\
• -s : specifies the random seed for the random state initialization (default: the seconds since the\
UNIX epoch, given by time(NULL)).\n\
• -v : enables verbose output.\n\
• -h : displays program synopsis and usage.\n");
            return 0;
        }
    }

    mpz_t p, q, pq, pub, priv;
    mpz_inits(p, q, pq, pub, priv, NULL);
    FILE *inpbfile;
    FILE *inpvfile;
    inpbfile = fopen(pbfile, "w"); //open the files for writing
    inpvfile = fopen(pvfile, "w");
    //set file permissions
    int fd = fileno(inpvfile);
    fchmod(fd, S_IRUSR | S_IWUSR);
    randstate_init(seed); //set state
    //make public and private keys
    ss_make_pub(p, q, pub, bits, iterations);
    ss_make_priv(priv, pq, p, q);
    char *username = getenv("USER");
    ss_write_pub(pub, username, inpbfile); //write public key to file
    ss_write_priv(pq, priv, inpvfile); //write private key
    if (verbose) {
        printf("user = %s\n", username);
        //print signature?
        gmp_printf("p (%u bits) = %Zd\nq (%u bits) = %Zd\nn (%u bits) = %Zd\n",
            mpz_sizeinbase(p, 2), p, mpz_sizeinbase(q, 2), q, mpz_sizeinbase(pub, 2), pub);
        //private exponent?
        gmp_printf("pq (%u bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%u bits) = %Zd\n", mpz_sizeinbase(priv, 2), priv);
    }
    fclose(inpvfile);
    fclose(inpbfile);
    randstate_clear();
    mpz_clears(p, q, pq, pub, priv, NULL);
    return 0;
}
