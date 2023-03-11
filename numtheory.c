#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, a1, b1; //initialize t
    mpz_inits(t, a1, b1, NULL);
    mpz_set(a1, a); //make a copy of a
    mpz_set(b1, b); //make a copy of b
    while (mpz_cmp_ui(b1, 0) != 0) { //while b != 0
        mpz_set(t, b1); // t = b
        mpz_mod(b1, a1, b1);
        mpz_set(a1, t); //a = t;
    }
    mpz_set(g, a1); //g = a
    mpz_clears(t, a1, b1, NULL); //clear all memory
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, rp, t, tp, q, temp, temp1;
    mpz_inits(r, rp, t, tp, q, temp, temp1, NULL);
    mpz_set(r, n); //r=n
    mpz_set(rp, a); //rp=a
    mpz_set_ui(t, 0); //t=0
    mpz_set_ui(tp, 1); //tp=1
    while (mpz_cmp_ui(rp, 0) != 0) { //while rp!=0
        mpz_fdiv_q(q, r, rp); //q=r/rp
        mpz_set(temp, r); //temp=r
        mpz_set(r, rp); //r=rp
        mpz_mul(temp1, q, rp); //remp1=q*rp
        mpz_sub(rp, temp, temp1); //rp=temp-temp1
        mpz_set(temp, t); //temp=t
        mpz_set(t, tp); //t=tp
        mpz_mul(temp1, q, tp); //temp1 = q*tp
        mpz_sub(tp, temp, temp1); //tp=temp-temp1
    }
    if (mpz_cmp_ui(r, 1) > 0) { //if r>1
        mpz_set_ui(o, 0); //o=0
    } else {
        if (mpz_cmp_ui(t, 0) <= 0) { //if t<=0
            mpz_add(o, t, n); //o=t+n
        } else {
            mpz_set(o, t); //o=t
        }
    }
    //clear all allocated memory
    mpz_clears(r, rp, t, tp, q, temp, temp1, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v, p, i, two;
    mpz_inits(v, p, i, two, NULL);

    mpz_set_ui(v, 1); //v=1
    mpz_set(p, a); //p=a
    mpz_set(i, d); //i=d
    mpz_set_ui(two, 2); //two=2

    while (mpz_cmp_ui(i, 0) > 0) { //while i >0
        if (mpz_odd_p(i)) { //if i is odd
            mpz_mul(v, v, p); //v=v*p
            mpz_mod(v, v, n); //v=v%n
        }
        mpz_mul(p, p, p); //p=p*p
        mpz_mod(p, p, n); //p=p%n
        mpz_fdiv_q(i, i, two); //i=i/2
    }
    mpz_set(o, v); //set o to v
    mpz_clears(v, p, i, two, NULL); //clear all memory
}

bool is_prime(const mpz_t n, uint64_t iters) {
    if (mpz_cmp_ui(n, 4) < 0) {
        return (mpz_cmp_ui(n, 1) > 0);
    }
    mpz_t r, s, s1, t, n2, n1, a, two, i, j, y;
    mpz_inits(r, s, s1, t, n2, n1, a, two, i, j, y, NULL);
    mpz_sub_ui(r, n, 1); //r = n-1
    mpz_set_ui(s, 0); //s=0
    mpz_sub_ui(n2, n, 3); //n2 = n-3

    mpz_sub_ui(n1, n, 1); //n1=n-1
    //write r such that r is odd
    while (mpz_even_p(r)) { //while r is even
        mpz_add_ui(s, s, 1); //s = s+1
        mpz_fdiv_q_ui(r, r, 2); //r= r/2
    }
    mpz_set_ui(two, 2); //two = 2
    mpz_sub_ui(s1, s, 1); //s1 = s-1
    //for i to iters
    for (mpz_set_ui(i, 1); mpz_cmp_ui(i, iters) <= 0; mpz_add_ui(i, i, 1)) {
        //choose random a from 2 to n-2
        mpz_urandomm(a, state, n2); //a=random 0 to n-4 because n2 is already n-3
        mpz_add_ui(a, a, 2); //add 2 to a to get 2 to n-2
        pow_mod(y, a, r, n); //y=powmod (a, r, n)
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n1) != 0)) { //if y!=1 and y!=n-1
            mpz_set_ui(j, 1); //j=1
            //while j<=s-1 and y != n-1
            while ((mpz_cmp(j, s1) <= 0) && (mpz_cmp(y, n1) != 0)) {
                pow_mod(y, y, two, n); //y = powmod(y, 2, n)
                if (mpz_cmp_ui(y, 1) == 0) { //if y==1
                    mpz_clears(r, s, s1, t, n2, n1, a, two, i, j, y, NULL);
                    return 0;
                }
                mpz_add_ui(j, j, 1); // j += 1
            }
            if (mpz_cmp(y, n1) != 0) { //if y!=n-1
                mpz_clears(r, s, s1, t, n2, n1, a, two, i, j, y, NULL);
                return 0;
            }
        }
    }
    mpz_clears(r, s, s1, t, n2, n1, a, two, i, j, y, NULL);
    return 1;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t num;
    mpz_init(num);
    mpz_urandomb(num, state, bits); //n=random number thats bits long
    //set the most and least significant bit of n to 1 to make sure it's odd
    mpz_setbit(num, bits - 1);
    mpz_setbit(num, 0);
    //while num isn't prime
    while (!is_prime(num, iters)) {
        mpz_urandomb(num, state, bits); //n=random number thats bits long
        //set the most and least significant bit of n to 1 to make sure it's odd
        mpz_setbit(num, bits - 1);
        mpz_setbit(num, 0);
    }
    mpz_set(p, num); //p=num
    mpz_clear(num); //clear allocated memory
}
