#include "ss.h"
#include "numtheory.h"
#include <stdlib.h>

void lcm(mpz_t r, mpz_t a, mpz_t b) { // for use in other functions
    mpz_t ab, gcdab;
    mpz_inits(ab, gcdab, NULL);
    mpz_mul(ab, a, b); //ab = a*b
    gcd(gcdab, a, b);
    mpz_fdiv_q(r, ab, gcdab); //r = ab/gcdab
    mpz_clears(ab, gcdab, NULL);
}

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    uint64_t pbits, qbits;
    mpz_t p1, q1, pdiv, qdiv, pq;
    mpz_inits(p1, q1, pdiv, qdiv, pq, NULL);
    pbits = (random() % (nbits / 5)) + nbits / 5; //pbits=[nbits/5, (2*nbits)/5]
    qbits = nbits - (2 * pbits); //qbits = nbits-2*pbits
    do {
        make_prime(p, pbits, iters); //make p a prime number
        make_prime(q, qbits, iters); //make q a prime number
        mpz_sub_ui(p1, p, 1); //p1 = p-1
        mpz_sub_ui(q1, q, 1); //q1 = q-1
        mpz_mod(pdiv, p, q1); //pdiv = p%q1
        mpz_mod(qdiv, q, p1); //qdiv = q%p1
    } while ((mpz_cmp_ui(pdiv, 0) == 0) || (mpz_cmp_ui(qdiv, 0) == 0));
    mpz_mul(pq, p, q); //pq = p*q
    mpz_mul(n, pq, p); //n=p*p*q
    mpz_clears(p1, q1, pdiv, qdiv, pq, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t p1, q1, lc, n;
    mpz_inits(p1, q1, lc, n, NULL);
    mpz_sub_ui(p1, p, 1); //p1 = p-1
    mpz_sub_ui(q1, q, 1); //q1 = q-1
    mpz_mul(pq, p, q); //pq = p*q
    mpz_mul(n, pq, p); //n=p*p*q
    lcm(lc, p1, q1); //lc = lcm p-1 q-1
    mod_inverse(d, n, lc); //private key = inverse of n mod lc
    mpz_clears(p1, q1, lc, n, NULL);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%ZX\n", n); //print public key as hex
    fprintf(pbfile, "%s\n", username); //print the username
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%ZX\n", pq); //print private key as hex
    gmp_fprintf(pvfile, "%ZX\n", d); //print pq as hex
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    //read the file setting n to the hex and username
    gmp_fscanf(pbfile, "%ZX\n%s", n, username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%ZX\n%ZX\n", pq, d); //set pq and d
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    mpz_powm(c, m, n, n); //c= m^n % n
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    //create block size
    mpz_t m, temp;
    mpz_inits(m, temp, NULL);
    mpz_sqrt(temp, n); //temp = sqrt(n)
    //ask if this is how to find the log
    uint8_t k = mpz_sizeinbase(temp, 2) - 1; //k = log2(temp)
    k -= 1;
    //ask if i need to subtract 1 again
    k = k / 8;
    //make block array of size k
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));
    block[0] = 0xFF; //set zeroth byte of the block
    //use fread()
    while (!feof(infile)) { //while still unprocessed bytes
        int j = fread(&block[1], sizeof(uint8_t), k - 1, infile);
        mpz_import(m, j + 1, 1, sizeof(block[0]), 1, 0, block); //create m
        ss_encrypt(m, m, n); //encrypt m and store it in m
        gmp_fprintf(outfile, "%ZX\n", m); //write ecrypted m to the outfile
    }
    //free all alocated memory
    mpz_clears(m, temp, NULL);
    free(block);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    mpz_powm(m, c, d, pq); //m = c^d % pq
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    //create block size
    mpz_t c, m, temp;
    mpz_inits(c, m, temp, NULL);
    //ask if this is how to find the log
    mpz_set(temp, pq); //temp = pq
    uint8_t k = mpz_sizeinbase(temp, 2) - 1; //k = log2(temp)
    //ask if i need to subtract 1 again
    k -= 1;
    k = k / 8;

    //make block array of size k
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));

    while (!feof(infile)) {
        gmp_fscanf(infile, "%ZX\n", c); //scan in a hexstring and save as c
        ss_decrypt(m, c, d, pq); //m = decrypted c
        //learn more about export
        uint64_t j = 0;
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, m); //generate array with m
        fwrite(&block[1], j - 1, sizeof(uint8_t), outfile);
    }
    //free all alocated memory
    mpz_clears(c, m, temp, NULL);
    free(block);
}
