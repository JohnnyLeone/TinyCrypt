/* This program is ARC4-DROP(1024) in it's most basic form. */

#include <stdio.h>
#include <string.h>
#include "encrypt.c" /* Note that we include the source directy for speed */

static char *password;
static FILE *randFile;
static int encrypting;

static unsigned char *genNonce(void)
{
    static unsigned char nonce[NONCE_LENGTH];
    int xByte;

    for(xByte = 0; xByte < NONCE_LENGTH; xByte++) {
        nonce[xByte] = getc(randFile);
    }
    return nonce;
}

static void encryptFile(
    FILE *inputFile,
    FILE *outputFile)
{
    unsigned char *nonce = genNonce();
    int xChar;
    int c;

    initKey(password, nonce, NONCE_LENGTH);
    /* Add the nonce */
    for(xChar = 0; xChar < NONCE_LENGTH; xChar++) {
        c = nonce[xChar];
        putc(c, outputFile);
    }
    c = getc(inputFile);
    while(c != EOF) {
        putc(hashChar(c), outputFile);
        c = getc(inputFile);
    }
}

static unsigned char *readNonce(
    FILE *file)
{
    static unsigned char buffer[NONCE_LENGTH];
    int xByte;

    for(xByte = 0; xByte < NONCE_LENGTH; xByte++) {
        buffer[xByte] = getc(file);
    }
    return buffer;
}

static void decryptFile(
    FILE *inputFile,
    FILE *outputFile)
{
    unsigned char *nonce = readNonce(inputFile);
    int c;

    initKey(password, nonce, NONCE_LENGTH);
    c = getc(inputFile);
    while(c != EOF) {
        putc(hashChar(c), outputFile);
        c = getc(inputFile);
    }
}

int main(int argc, char**argv) {
    char *extension, *inFileName, *outFileName;
    FILE *inputFile, *outputFile;

    if(argc != 3) {
        printf("Usage: %s password file\n", argv[0]);
        return 1;
    }
    inFileName = argv[2];
    password = argv[1];
    outFileName = calloc(strlen(inFileName) + 5, sizeof(char));
    strcpy(outFileName, inFileName);
    extension = strrchr(outFileName, '.');
    if(extension == NULL || strcasecmp(extension, ".enc")) {
        strcat(outFileName, ".enc");
        encrypting = 1;
    } else {
        *extension = '\0';
        encrypting = 0;
    }
    inputFile = fopen(inFileName, "rb");
    if(inputFile == NULL) {
        printf("Could not open file %s for reading\n", inFileName);
        return 1;
    }
    outputFile = fopen(outFileName, "wb");
    if(outputFile == NULL) {
        printf("Could not open file %s for writing\n", outFileName);
        return 1;
    }
    randFile = fopen("/dev/urandom", "r");
    if(randFile == NULL) {
        printf("Unable to open random number source.\n");
        return 1;
    }
    if(encrypting) {
        encryptFile(inputFile, outputFile);
        clearKey();
    } else {
        decryptFile(inputFile, outputFile);
        clearKey();
    }
    fclose(outputFile);
    fclose(inputFile);
    fclose(randFile);
    return 0;
}
