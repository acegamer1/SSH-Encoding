#include "ss.c"
#include "numtheory.c"
#include "randstate.c"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define OPTIONS "i:o:n:vh"

char username[100];
char pbfile[100] = "ss.pub";
char input[100];
char output[100];
int verbose = 0;
int casei = 0;
int caseo = 0;

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
        case 'n': strcpy(pbfile, optarg); break;
        case 'v': ++verbose; break;
        case 'h':
            printf("• -i : specifies the input file to encrypt (default: stdin).\n\
• -o : specifies the output file to encrypt (default: stdout).\n\
• -n : specifies the file containing the public key (default: ss.pub).\n\
• -v : enables verbose output.\n\
• -h : displays program synopsis and usage");
            return 0;
        }
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
    mpz_t pubkey;
    mpz_inits(pubkey, NULL);
    FILE *inpbfile;
    inpbfile = fopen(pbfile, "r");
    //ask how to upen ss.pub so that i can see username
    ss_read_pub(pubkey, username, inpbfile);
    if (verbose) {
        printf("user = %s\n", username);
        gmp_printf("n (%u bits) = %Zd\n", mpz_sizeinbase(pubkey, 2), pubkey);
    }
    ss_encrypt_file(infile, outfile, pubkey);
    if (casei) {
        fclose(infile);
    }
    if (caseo) {
        fclose(outfile);
    }
    fclose(inpbfile);
    mpz_clears(pubkey, NULL);
    return 0;
}
