/*
*   A byte-oriented AES-256-CTR implementation.
*   Based on the code available at http://www.literatecode.com/aes256
*   Complies with RFC3686, http://tools.ietf.org/html/rfc3686
*
*   This demo uses RFC3686 Test Vector #9.
*
*/
#include <stdlib.h>
#include <stdio.h>
#include "aes256.h"

void dump(char *s, uint8_t *buf, size_t sz);

void dump(char *s, uint8_t *buf, size_t sz)
{
   size_t i;
   printf("%s",s); for(i=0;i<sz;i++) printf("%02X ",buf[i]); printf("\n");
} /* dump */


int main (int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
    aes256_context ctx;
    uint8_t key[32] = {
                        0xFF, 0x7A, 0x61, 0x7C, 0xE6, 0x91, 0x48, 0xE4,
                        0xF1, 0x72, 0x6E, 0x2F, 0x43, 0x58, 0x1D, 0xE2,
                        0xAA, 0x62, 0xD9, 0xF8, 0x05, 0x53, 0x2E, 0xDF,
                        0xF1, 0xEE, 0xD6, 0x87, 0xFB, 0x54, 0x15, 0x3D
                      };
    uint8_t buf[] = {
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
                        0x20, 0x21, 0x22, 0x23
                      };
    rfc3686_blk ctr = {
                        {0x00, 0x1C, 0xC5, 0xB7}, /* nonce */
                        {0x51, 0xA5, 0x1D, 0x70, 0xA1, 0xC1, 0x11, 0x48}, /* IV */
                        {0x00, 0x00, 0x00, 0x01} /* counter */
                      };

    dump("txt: ", buf, sizeof(buf));
    dump("key: ", key, sizeof(key));
    printf("---\n");

    aes256_init(&ctx, key);
    aes256_setCtrBlk(&ctx, &ctr);

    aes256_encrypt_ctr(&ctx, buf, sizeof(buf));
    dump("enc: ", buf, sizeof(buf));
    printf("chk: EB 6C 52 82 1D 0B BB F7 CE 75 94 46 2A CA 4F AA"
               " B4 07 DF 86 65 69 FD 07 F4 8C C0 B5 83 D6 07 1F"
               " 1E C0 E6 B8\n");


    ctr.ctr[0] = ctr.ctr[1] = ctr.ctr[2] = 0, ctr.ctr[3] = 1;
    aes256_setCtrBlk(&ctx, &ctr);
    aes256_decrypt_ctr(&ctx, buf, sizeof(buf));
    dump("dec: ", buf, sizeof(buf));

    aes256_done(&ctx);

    return 0;
} /* main */
