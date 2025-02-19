/*
 * Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm
 * and modified slightly to be functionally identical but condensed into control structures.
 * see -> https://github.com/Zunawe/md5-c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>


typedef struct{
    uint64_t size;        // Size of input in bytes
    uint32_t buffer[4];   // Current accumulation of hash
    uint8_t input[64];    // Input to be used in the next step
    uint8_t digest[16];   // Result of algorithm
}MD5Context;

/*
 * Constants defined by the MD5 algorithm
 */

static uint32_t S[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                       5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                       4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                       6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

static uint32_t K[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                       0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                       0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                       0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                       0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                       0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                       0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                       0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                       0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                       0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                       0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                       0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                       0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                       0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                       0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                       0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

/*
 * Padding used to make the size (in bits) of the input congruent to 448 mod 512
 */
static uint8_t PADDING[] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*
 * Bit-manipulation functions defined by the MD5 algorithm
 */
#define F(X, Y, Z) ((X & Y) | (~X & Z))
#define G(X, Y, Z) ((X & Z) | (Y & ~Z))
#define H(X, Y, Z) (X ^ Y ^ Z)
#define I(X, Y, Z) (Y ^ (X | ~Z))

/*
 * Rotates a 32-bit word left by n bits
 */
uint32_t rotateLeft(uint32_t x, uint32_t n){
    return (x << n) | (x >> (32 - n));
}


/*
 * Initialize a context
 */
void md5Init(MD5Context *ctx, int32_t *IV){
    ctx->size = (uint64_t)0;

    ctx->buffer[0] = (uint32_t)IV[0];
    ctx->buffer[1] = (uint32_t)IV[1];
    ctx->buffer[2] = (uint32_t)IV[2];
    ctx->buffer[3] = (uint32_t)IV[3];
}

/*
 * Step on 512 bits of input with the main MD5 algorithm.
 */
void md5Step(uint32_t *buffer, uint32_t *input){
    uint32_t AA = buffer[0];
    uint32_t BB = buffer[1];
    uint32_t CC = buffer[2];
    uint32_t DD = buffer[3];

    uint32_t E;

    unsigned int j;

    for(unsigned int i = 0; i < 64; ++i){
        switch(i / 16){
            case 0:
                E = F(BB, CC, DD);
                j = i;
                break;
            case 1:
                E = G(BB, CC, DD);
                j = ((i * 5) + 1) % 16;
                break;
            case 2:
                E = H(BB, CC, DD);
                j = ((i * 3) + 5) % 16;
                break;
            default:
                E = I(BB, CC, DD);
                j = (i * 7) % 16;
                break;
        }

        uint32_t temp = DD;
        DD = CC;
        CC = BB;
        BB = BB + rotateLeft(AA + E + K[i] + input[j], S[i]);
        AA = temp;
    }

    buffer[0] += AA;
    buffer[1] += BB;
    buffer[2] += CC;
    buffer[3] += DD;
}

/*
 * Add some amount of input to the context
 *
 * If the input fills out a block of 512 bits, apply the algorithm (md5Step)
 * and save the result in the buffer. Also updates the overall size.
 */
void md5Update(MD5Context *ctx, uint8_t *input_buffer, size_t input_len){
    uint32_t input[16];
    unsigned int offset = ctx->size % 64;
    ctx->size += (uint64_t)input_len;

    // Copy each byte in input_buffer into the next space in our context input
    for(unsigned int i = 0; i < input_len; ++i){
        ctx->input[offset++] = (uint8_t)*(input_buffer + i);

        // If we've filled our context input, copy it into our local array input
        // then reset the offset to 0 and fill in a new buffer.
        // Every time we fill out a chunk, we run it through the algorithm
        // to enable some back and forth between cpu and i/o
        if(offset % 64 == 0){
            for(unsigned int j = 0; j < 16; ++j){
                // Convert to little-endian
                // The local variable `input` our 512-bit chunk separated into 32-bit words
                // we can use in calculations
                input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
                           (uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
                           (uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
                           (uint32_t)(ctx->input[(j * 4)]);
            }
            md5Step(ctx->buffer, input);
            offset = 0;
        }
    }
}

/*
 * Pad the current input to get to 448 bytes, append the size in bits to the very end,
 * and save the result of the final iteration into digest.
 */
void md5Finalize(MD5Context *ctx){
    uint32_t input[16];
    unsigned int offset = ctx->size % 64;
    unsigned int padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

    // Fill in the padding and undo the changes to size that resulted from the update
    md5Update(ctx, PADDING, padding_length);
    ctx->size -= (uint64_t)padding_length;

    // Do a final update (internal to this function)
    // Last two 32-bit words are the two halves of the size (converted from bytes to bits)
    for(unsigned int j = 0; j < 14; ++j){
        input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
                   (uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
                   (uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
                   (uint32_t)(ctx->input[(j * 4)]);
    }
    input[14] = (uint32_t)(ctx->size * 8);
    input[15] = (uint32_t)((ctx->size * 8) >> 32);

    md5Step(ctx->buffer, input);

    // Move the result into digest (convert from little-endian)
    for(unsigned int i = 0; i < 4; ++i){
        ctx->digest[(i * 4) + 0] = (uint8_t)((ctx->buffer[i] & 0x000000FF));
        ctx->digest[(i * 4) + 1] = (uint8_t)((ctx->buffer[i] & 0x0000FF00) >>  8);
        ctx->digest[(i * 4) + 2] = (uint8_t)((ctx->buffer[i] & 0x00FF0000) >> 16);
        ctx->digest[(i * 4) + 3] = (uint8_t)((ctx->buffer[i] & 0xFF000000) >> 24);
    }
}

/*
 * Functions that run the algorithm on the provided input and put the digest into result.
 * result should be able to store 16 bytes.
 */
void md5String(char *input, uint8_t *result, int32_t *IV){
    MD5Context ctx;

    int32_t def_IV[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    if (IV == NULL) {
        md5Init(&ctx, def_IV);
    }
    else {
        md5Init(&ctx, IV);
    }

    md5Update(&ctx, (uint8_t *)input, strlen(input));
    md5Finalize(&ctx);

    memcpy(result, ctx.digest, 16);
}

void md5File(FILE *file, uint8_t *result, int32_t *IV){
    char *input_buffer = malloc(1024);
    size_t input_size = 0;

    MD5Context ctx;
    md5Init(&ctx, IV);

    while((input_size = fread(input_buffer, 1, 1024, file)) > 0){
        md5Update(&ctx, (uint8_t *)input_buffer, input_size);
    }

    md5Finalize(&ctx);

    free(input_buffer);

    memcpy(result, ctx.digest, 16);
}

void print_hash(uint8_t *p){
    for(unsigned int i = 0; i < 16; ++i){
        printf("%02x", p[i]);
    }
    printf("\n");
}
void print_bytes(uint8_t *p, int size){
    for(unsigned int i = 0; i < size; ++i){
        printf("%02x", p[i]);
    }
    printf("\n");
}

void hexStringToUInt32Array(const char *hexString, uint32_t *outputArray) {
    // Chaque entier de 32 bits (4 octets) est représenté par 8 caractères hexadécimaux
    for (int i = 0; i < 4; ++i) {
        // Extraire un bloc de 8 caractères (32 bits) et le convertir en entier
        char buffer[9];
        snprintf(buffer, sizeof(buffer), "%.8s", hexString + i * 8);
        
        // Convertir la chaîne hexadécimale en un entier de 32 bits
        outputArray[i] = (uint32_t)strtoul(buffer, NULL, 16);
    }
}
void hexStringToBytesArray(char *hexString, int size_bytes, char *outputArray) {
    // Chaque octet est représenté par 2 caractères hexadécimaux
    for (int i = 0; i < size_bytes; ++i) {
        // Extraire un bloc de 8 caractères (32 bits) et le convertir en entier
        char buffer[3];
        buffer[0] = hexString[i*2];
        buffer[1] = hexString[i*2 + 1];
        buffer[2] = 0x00;

        // Convertir la chaîne hexadécimale en un entier de 32 bits
        outputArray[i] = (char)strtoul(buffer, NULL, 16);
    }
}
void digestToBytes(struct md5_digest *digest, int32_t *outputArray) {
    // Chaque entier de 32 bits (4 octets) est représenté par 8 caractères hexadécimaux
    for (int i = 0; i < 4; ++i) {
        outputArray[i] = 0;
        outputArray[i] =     (int32_t)(digest->bytes[i * 4] << 24     ) |
                            ((int32_t)(digest->bytes[i * 4 + 1]) << 16) |
                            ((int32_t)(digest->bytes[i * 4 + 2]) << 8 ) |
                             (int32_t)(digest->bytes[i * 4 + 3]);
    }
}


// write H(key || name || expiry) in result, 
int hash_tag(char *key, char *name, time_t expiry, char *result) {

    struct tm *timeinfo;
    char time_format[11]; // Enough space for "YYYY-MM-DD\0"
    // Convert time_t to struct tm (local time)
    timeinfo = localtime(&expiry);
    // Format the date as "YYYY-MM-DD"
    strftime(time_format, 11, "%Y-%m-%d", timeinfo);

    char buffer[192];
    bzero(buffer, 192);
    memcpy(buffer, key, 64);
    snprintf(&buffer[64], 128, "%s#%s", name, time_format);

    // printf("Prepare to hash : \n\t - key : ");
    // print_bytes(key, 64);
    // printf("\t - name : %s\n", name);
    // printf("\t - time : %s\n", time_format);

    printf("\nBuffer[192] : ");
    print_bytes(buffer, 192);

    md5String(buffer, result, NULL);
    return 0;
}

int main(int argc, char *argv[]){
    char *value = "bonjour";
    uint8_t result[16];

    // A 0x67452301
    // B 0xefcdab89
    // C 0x98badcfe
    // D 0x10325476

    // int32_t mine[4];
    // hexStringToUInt32Array("abababab01020304efefefef79797979", mine);
    // printf("\n1: %08x\n", mine[0]);
    // printf("\n2: %08x\n", mine[1]);
    // printf("\n3: %08x\n", mine[2]);
    // printf("\n4: %08x\n", mine[3]);

    // REVERSE TIME FORMAT
    // if (strptime(date_str, "%Y-%m-%d", &timeinfo) == NULL) {
    //     fprintf(stderr, "Error parsing date.\n");
    //     return (time_t)(-1);
    // }


    // la clef
    char key[64];
    memset(key, 'A', 64);
    // le nom
    char name[64];
    memset(name, 'Z', 64);
    // la date d'expiration
    time_t mytime = time(NULL);
    // hachage
    // hash_tag(key, "name", mytime, result);
    md5String(key, result, NULL);
    print_hash(result);

    int32_t mine[4];
    hexStringToUInt32Array("d289a97565bc2d27ac8b8545a5ddba45", mine);
    md5String("namenamenamenamenamenamenamenamenamenamenamenamenamen#2024-10-27", result, mine);
    print_hash(result);
    
    hash_tag(key, "namenamenamenamenamenamenamenamenamenamenamenamenamen", mytime, result);
    print_hash(result);


    // int32_t IV[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    // md5String(hex_buffer, result, NULL);
    


    return 0;
}

