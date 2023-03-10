# Public Key Cryptography

## Description:
This program is a public/private key generator. These keys can be used to encrypt and decrypt data. The public key is used\
by a sender of data to encrypt their data and the private key is used by the receiver to decrypt the data.

## Building Executables:
To build all executables make sure to first download all files and enter the directory of those files and run\
$ make or $ make all. To make executables for only one program run one of the following options to make the\
corresponding program executable; $ make decrypt, $ make encrypt, $ make keygen. In order to format all files run\
$ make format.

## Cleaning:
To clean the directory of all executable run the command $ make clean.

## Run:
Once you have made the executables that you want to run use the command, $ ./(executable) 'argument(s)'. In order \
to see the available arguments and usage, simply run that executable with the argument -h.

## Available Command Line Arguments for Decrypt Executable:
• -i : specifies the input file to decrypt (default: stdin).\
• -o : specifies the output file to decrypt (default: stdout).\
• -n : specifies the file containing the private key (default: ss.priv).\
• -v : enables verbose output.\
• -h : displays program synopsis and usage.

## Available Command Line Arguments for Encrypt Executable:
• -i : specifies the input file to encrypt (default: stdin).\
• -o : specifies the output file to encrypt (default: stdout).\
• -n : specifies the file containing the public key (default: ss.pub).\
• -v : enables verbose output.\
• -h : displays program synopsis and usage.

## Available Command Line Arguments for Keygen Executable:
• -b : specifies the minimum bits needed for the public modulus n.\
• -i : specifies the number of Miller-Rabin iterations for testing primes (default: 50).\
• -n pbfile : specifies the public key file (default: ss.pub).\
• -d pvfile : specifies the private key file (default: ss.priv).\
• -s : specifies the random seed for the random state initialization (default: the seconds since the UNIX epoch,\
given by time(NULL)).
• -v : enables verbose output.\
• -h : displays program synopsis and usage.

## Scan-Build False Positives:


## Bugs and Errors:
