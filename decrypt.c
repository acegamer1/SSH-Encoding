#include "ss.c"
#include "numtheory.c"
#include "randstate.c"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define OPTIONS "i:o:n:vh"

char pvfile[100] = "ss.priv";
char input[100];
char output[100];
int casei = 0;
int caseo = 0;
int verbose = 0;

int main(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            ++casei;
            strcpy(input, optarg); //set input file
            break;
        case 'o':
            ++caseo;
            strcpy(output, optarg); //set ouput file
            break;
        case 'n': strcpy(pvfile, optarg); break;
        case 'v': ++verbose; break;
        case 'h':
            printf("• -i : specifies the input file to decrypt (default: stdin).\n\
• -o : specifies the output file to decrypt (default: stdout).\n\
• -n : specifies the file containing the private key (default: ss.priv).\n\
• -v : enables verbose output.\n\
• -h : displays program synopsis and usage");
            return 0;
        }
    }
    mpz_t privkey, pq;
    mpz_inits(privkey, pq, NULL);
    FILE *inpvfile;
    inpvfile = fopen(pvfile, "r");
    ss_read_priv(pq, privkey, inpvfile); //read private key and pq
    if (verbose) {
        gmp_printf("pq (%u bits) = %Zd\nd (%u bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq,
            mpz_sizeinbase(privkey, 2), privkey);
    }
    FILE *infile;
    FILE *outfile;
    if (!casei) {
        infile = stdin;
    } else {
        infile = fopen(input, "r");
    }
    if (!caseo) {
        outfile = stdout;
    } else {
        outfile = fopen(output, "w");
    }
    ss_decrypt_file(infile, outfile, privkey, pq); //decrypt file
    //close all opened files
    if (casei) {
        fclose(infile);
    }
    if (caseo) {
        fclose(outfile);
    }
    fclose(inpvfile);
    mpz_clears(privkey, pq, NULL);
    return 0;
}
