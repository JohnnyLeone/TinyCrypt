/* Test that the encrypted data produced by SimpleCrypt match expected test values posted on
 * Wikipedia.org:
 *     RC4( "Key", "Plaintext" ) == BBF316E8D940AF0AD3
 *     RC4( "Wiki", "pedia" ) == 1021BF0420
 *     RC4( "Secret", "Attack at dawn" ) == 45A01F645FC35B383552544B9BF5 */

#include <stdio.h>
#include <stdlib.h>
#include "encrypt.c" /* Note that we include encrypt.c, so we can have better optimization */

/* Just convert the 4-bit value to a hex digit. */
static char toHex(char value) {
    if(value <= 9) {
        return value + '0';
    }
    return value + 'A' - 10;
}

/* Check a particular test vector.  Note: hex results must be upper case. */
static void checkARC4(char *key, char *plaintext, char *hexResult) {
    char encryptedChar;

    initKey(key, NULL, 0);
    while(*plaintext) {
        encryptedChar = hashChar(*plaintext);
        plaintext++;
        if(toHex((encryptedChar >> 4) & 0xf) != *hexResult++) {
            printf("ARC4 encryption failed on key '%s'!\n", key);
            exit(1);
        }
        if(toHex(encryptedChar & 0xf) != *hexResult++) {
            printf("ARC4 encryption failed on key '%s'!\n", key);
            exit(1);
        }
    }
}

int main(void) {
    checkARC4("Key", "Plaintext", "BBF316E8D940AF0AD3");
    checkARC4("Wiki", "pedia", "1021BF0420");
    checkARC4("Secret", "Attack at dawn", "45A01F645FC35B383552544B9BF5");
    printf("ARC4 tests passed.\n");
    return 0;
}
