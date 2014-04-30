/*
*   A byte-oriented AES-256-CTR implementation.
*   Based on the code available at http://www.literatecode.com/aes256
*   Complies with RFC3686, http://tools.ietf.org/html/rfc3686
*
*   This demo uses RFC3686 Test Vector #9 for CTR and test vector from
*   FIPS PUB 197, Appendix C.3 for core ECB
*
*/
#include <stdlib.h>
#include <stdio.h>
#include "aes256.h"

void dump(char *s, uint8_t *buf, size_t sz);
int mem_isequal(const uint8_t *x, const uint8_t *y, size_t sz);


static const uint8_t RFC3686_TV9[] =
{
    0xEB, 0x6C, 0x52, 0x82, 0x1D, 0x0B, 0xBB, 0xF7,
    0xCE, 0x75, 0x94, 0x46, 0x2A, 0xCA, 0x4F, 0xAA,
    0xB4, 0x07, 0xDF, 0x86, 0x65, 0x69, 0xFD, 0x07,
    0xF4, 0x8C, 0xC0, 0xB5, 0x83, 0xD6, 0x07, 0x1F,
    0x1E, 0xC0, 0xE6, 0xB8
};

static const uint8_t AES256_TV[] =
{
    0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf,
    0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89
};


int main (int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
    uint8_t key[32] =
    {
        0xFF, 0x7A, 0x61, 0x7C, 0xE6, 0x91, 0x48, 0xE4,
        0xF1, 0x72, 0x6E, 0x2F, 0x43, 0x58, 0x1D, 0xE2,
        0xAA, 0x62, 0xD9, 0xF8, 0x05, 0x53, 0x2E, 0xDF,
        0xF1, 0xEE, 0xD6, 0x87, 0xFB, 0x54, 0x15, 0x3D
    };
    uint8_t buf[] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23
    };
    rfc3686_blk ctr =
    {
        {0x00, 0x1C, 0xC5, 0xB7}, /* nonce */
        {0x51, 0xA5, 0x1D, 0x70, 0xA1, 0xC1, 0x11, 0x48}, /* IV */
        {0x00, 0x00, 0x00, 0x01} /* counter */
    };
    aes256_context ctx;
    uint8_t i;

    /*  **********************************************************
    *   First we test CTR
    */

    printf("# CTR test\n");
    dump("Text:", buf, sizeof(buf));
    dump("Key:", key, sizeof(key));

    aes256_init(&ctx, key);
    aes256_setCtrBlk(&ctx, &ctr);

    aes256_encrypt_ctr(&ctx, buf, sizeof(buf));
    dump("Result:", buf, sizeof(buf));
    if ( mem_isequal(buf, RFC3686_TV9, sizeof(RFC3686_TV9)) == 0 )
        printf("\t^ Ok\n");
    else
        printf("\t^ INVALID\n");

    /* reset the counter to decrypt */
    ctr.ctr[0] = ctr.ctr[1] = ctr.ctr[2] = 0, ctr.ctr[3] = 1;
    aes256_setCtrBlk(&ctx, &ctr);
    aes256_decrypt_ctr(&ctx, buf, sizeof(buf));
    dump("Text:", buf, sizeof(buf));


    /* ************************************************************
    *   Now we test the core AES-256 ECB, just in case
    */

    printf("\n# ECB test\n");
    for (i = 0; i < sizeof(AES256_TV); i++) buf[i] = ((i << 4) & 0xF0) + i;
    for (i = 0; i < sizeof(key); i++) key[i] = i;
    dump("Text:", buf, sizeof(AES256_TV));
    dump("Key:", key, sizeof(key));
    aes256_init(&ctx, key);
    aes256_encrypt_ecb(&ctx, buf);
    dump("Result:", buf, sizeof(AES256_TV));
    if ( mem_isequal(buf, AES256_TV, sizeof(AES256_TV)) == 0 )
        printf("\t^ Ok\n");
    else
        printf("\t^ INVALID");

    aes256_done(&ctx);

    return 0;
} /* main */


/* -------------------------------------------------------------------------- */
void dump(char *s, uint8_t *buf, size_t sz)
{
    size_t i;

    printf("%s\n\t", s);
    for (i = 0; i < sz; i++)
        printf("%02x%s", buf[i], ((i % 16 == 15) && (i < sz - 1)) ? "\n\t" : " ");
    printf("\n");
} /* dump */


/* -------------------------------------------------------------------------- */
int mem_isequal(const uint8_t *x, const uint8_t *y, size_t sz)
{
    size_t i;
    int rv;

    if ((sz < 1) || (x == NULL) || (y == NULL) ) return -1;

    for (i = 0, rv = 0; i < sz; i++) rv |= (x[i] ^ y[i]);

    return rv;
} /* mem_isequal */
