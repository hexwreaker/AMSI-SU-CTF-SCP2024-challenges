#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MD5_HASH_SIZE 16

#define F(x, y, z) (z ^ (x & (y ^ z)))
#define G(x, y, z) (y ^ (z & (x ^ y)))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

#define ROTATE_LEFT(x, s) (x<<s | x>>(32-s))

#define STEP(f, a, b, c, d, x, t, s) ( \
    a += f(b, c, d) + x + t, \
    a = ROTATE_LEFT(a, s), \
    a += b \
)


struct md5_context {
    // state
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    // number of bits, modulo 2^64 (lsb first)
    unsigned int count[2];
    // input buffer
    unsigned char input[64];
    // current block
    unsigned int block[16];
};

struct md5_digest {
    unsigned char bytes[MD5_HASH_SIZE];
};

void md5_init(struct md5_context *ctx, int32_t *iv);
void md5_update(struct md5_context* ctx, const void* buffer, unsigned int buffer_size);
void md5_finalize(struct md5_context* ctx, struct md5_digest* digest);
int md5(const char* input, struct md5_digest *digest, int32_t *iv);

void md5_init(struct md5_context* ctx, int32_t *iv) {

    if (iv == NULL) {
        ctx->a = 0x67452301;
        ctx->b = 0xefcdab89;
        ctx->c = 0x98badcfe;
        ctx->d = 0x10325476;
    }
    else {
        ctx->a = iv[0];
        ctx->b = iv[1];
        ctx->c = iv[2];
        ctx->d = iv[3];
    }

    ctx->count[0] = 0;
    ctx->count[1] = 0;
}

uint8_t* md5_transform(struct md5_context* ctx, const void* data, uintmax_t size) {
    uint8_t* ptr = (uint8_t*) data;
    uint32_t a, b, c, d, aa, bb, cc, dd;

    #define GET(n) (ctx->block[(n)])
    #define SET(n) (ctx->block[(n)] =        \
          ((uint32_t)ptr[(n)*4 + 0] << 0 )   \
        | ((uint32_t)ptr[(n)*4 + 1] << 8 )   \
        | ((uint32_t)ptr[(n)*4 + 2] << 16)   \
        | ((uint32_t)ptr[(n)*4 + 3] << 24) )

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do {
        aa = a;
        bb = b;
        cc = c; 
        dd = d;

        STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7);
        STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12);
        STEP(F, c, d, a, b, SET(2), 0x242070db, 17);
        STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22);
        STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7);
        STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12);
        STEP(F, c, d, a, b, SET(6), 0xa8304613, 17);
        STEP(F, b, c, d, a, SET(7), 0xfd469501, 22);
        STEP(F, a, b, c, d, SET(8), 0x698098d8, 7);
        STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12);
        STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17);
        STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22);
        STEP(F, a, b, c, d, SET(12), 0x6b901122, 7);
        STEP(F, d, a, b, c, SET(13), 0xfd987193, 12);
        STEP(F, c, d, a, b, SET(14), 0xa679438e, 17);
        STEP(F, b, c, d, a, SET(15), 0x49b40821, 22);

        STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5);
        STEP(G, d, a, b, c, GET(6), 0xc040b340, 9);
        STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14);
        STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20);
        STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5);
        STEP(G, d, a, b, c, GET(10), 0x02441453, 9);
        STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14);
        STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20);
        STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5);
        STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9);
        STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14);
        STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20);
        STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5);
        STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9);
        STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14);
        STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20);

        STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4);
        STEP(H, d, a, b, c, GET(8), 0x8771f681, 11);
        STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16);
        STEP(H, b, c, d, a, GET(14), 0xfde5380c, 23);
        STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4);
        STEP(H, d, a, b, c, GET(4), 0x4bdecfa9, 11);
        STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16);
        STEP(H, b, c, d, a, GET(10), 0xbebfbc70, 23);
        STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4);
        STEP(H, d, a, b, c, GET(0), 0xeaa127fa, 11);
        STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16);
        STEP(H, b, c, d, a, GET(6), 0x04881d05, 23);
        STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4);
        STEP(H, d, a, b, c, GET(12), 0xe6db99e5, 11);
        STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16);
        STEP(H, b, c, d, a, GET(2), 0xc4ac5665, 23);

        STEP(I, a, b, c, d, GET(0), 0xf4292244, 6);
        STEP(I, d, a, b, c, GET(7), 0x432aff97, 10);
        STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15);
        STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21);
        STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6);
        STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10);
        STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15);
        STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21);
        STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6);
        STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10);
        STEP(I, c, d, a, b, GET(6), 0xa3014314, 15);
        STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21);
        STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6);
        STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10);
        STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15);
        STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21);

        a += aa;
        b += bb;
        c += cc;
        d += dd;

        ptr += 64;
    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    #undef GET
    #undef SET

    return ptr;
}

void md5_update(struct md5_context* ctx, const void* buffer, uint32_t buffer_size) {
    uint32_t saved_low = ctx->count[0];
    uint32_t used;
    uint32_t free;

    if ((ctx->count[0] = ((saved_low+buffer_size) & 0x1fffffff)) < saved_low) {
        ctx->count[1]++;
    }
    ctx->count[1] += (uint32_t)(buffer_size>>29);

    used = saved_low & 0x3f;

    if (used) {
        free = 64 - used;

        if (buffer_size < free) {
            memcpy(&ctx->input[used], buffer, buffer_size);
            return;
        }

        memcpy(&ctx->input[used], buffer, free);
        buffer = (uint8_t*) buffer + free;
        buffer_size -= free;
        md5_transform(ctx, ctx->input, 64);
    }

    if (buffer_size >= 64) {
        buffer = md5_transform(ctx, buffer, buffer_size & ~(unsigned long)0x3f);
        buffer_size = buffer_size % 64;
    }

    memcpy(ctx->input, buffer, buffer_size);
}

void md5_finalize(struct md5_context* ctx, struct md5_digest* digest) {
    uint32_t used = ctx->count[0] & 0x3f;
    ctx->input[used++] = 0x80;
    uint32_t free = 64 - used;

    if (free < 8) {
        memset(&ctx->input[used], 0, free);
        md5_transform(ctx, ctx->input, 64);
        used = 0;
        free = 64;
    }

    memset(&ctx->input[used], 0, free - 8);

    ctx->count[0] <<= 3;
    ctx->input[56] = (uint8_t)(ctx->count[0]);
    ctx->input[57] = (uint8_t)(ctx->count[0] >> 8);
    ctx->input[58] = (uint8_t)(ctx->count[0] >> 16);
    ctx->input[59] = (uint8_t)(ctx->count[0] >> 24);
    ctx->input[60] = (uint8_t)(ctx->count[1]);
    ctx->input[61] = (uint8_t)(ctx->count[1] >> 8);
    ctx->input[62] = (uint8_t)(ctx->count[1] >> 16);
    ctx->input[63] = (uint8_t)(ctx->count[1] >> 24);

    md5_transform(ctx, ctx->input, 64);

    digest->bytes[0]  = (uint8_t)(ctx->a);
    digest->bytes[1]  = (uint8_t)(ctx->a >> 8);
    digest->bytes[2]  = (uint8_t)(ctx->a >> 16);
    digest->bytes[3]  = (uint8_t)(ctx->a >> 24);
    digest->bytes[4]  = (uint8_t)(ctx->b);
    digest->bytes[5]  = (uint8_t)(ctx->b >> 8);
    digest->bytes[6]  = (uint8_t)(ctx->b >> 16);
    digest->bytes[7]  = (uint8_t)(ctx->b >> 24);
    digest->bytes[8]  = (uint8_t)(ctx->c);
    digest->bytes[9]  = (uint8_t)(ctx->c >> 8);
    digest->bytes[10] = (uint8_t)(ctx->c >> 16);
    digest->bytes[11] = (uint8_t)(ctx->c >> 24);
    digest->bytes[12] = (uint8_t)(ctx->d);
    digest->bytes[13] = (uint8_t)(ctx->d >> 8);
    digest->bytes[14] = (uint8_t)(ctx->d >> 16);
    digest->bytes[15] = (uint8_t)(ctx->d >> 24);
}

int md5(const char* input, struct md5_digest *digest, int32_t *iv) {
    struct md5_context context;

    md5_init(&context, iv);
    md5_update(&context, input, (unsigned long)strlen(input));
    md5_finalize(&context, digest);

    char* output = malloc(sizeof(char) * (sizeof(digest)*2 + 1));
    output[sizeof(digest)] = '\0';

    for(int i=0; i<sizeof(digest); i++)
    {
        sprintf(&output[i*2], "%2.2x", digest->bytes[i]);
    }

    return 0;
}

int hex_to_bytes(char *hex_str, char *output, int size) {
    for (int i = 0; i < size; i++) {
        char tmp[3];
        tmp[0] = hex_str[i*2];
        tmp[1] = hex_str[i*2+1];
        tmp[2] = 0;
        output[i] = (uint8_t)strtol(tmp, NULL, 16);
    }
    return 0;
}
void bytes_to_hex(const unsigned char *bytes, int len, char *output) {
    for (int i = 0; i < len; i++) {
        sprintf(&output[i*2], "%02x", bytes[i]);
    }
}

// concatene un bloc pret à être hasher par md5, les valeurs doivent etres encodées en hexa.
int build_blocks(char *key, char *name, char *date, char *dst) {
    // Key
    char key_bytes[64];
    hex_to_bytes(key, key_bytes, 64);
    memcpy(dst, key_bytes, 64); // la clef
    // Name
    char name_bytes[17];
    hex_to_bytes(name, name_bytes, 16);
    name_bytes[16] = 0;
    int len_name_bytes = strlen(name_bytes);
    memcpy(&dst[64], name_bytes, len_name_bytes);
    // Date
    char date_bytes[11];
    hex_to_bytes(date, date_bytes, 10);
    date_bytes[10] = 0;
    memcpy(&dst[64+len_name_bytes], date_bytes, strlen(date_bytes));
    return 64+len_name_bytes+10;
}

time_t get_time_t_from_date(const char *date_str) {
    struct tm timeinfo = {0};  // Initialize to zero to avoid undefined behavior

    // Parse the date string into the tm structure
    if (strptime(date_str, "%Y-%m-%d", &timeinfo) == NULL) {
        fprintf(stderr, "Error parsing date.\n");
        return (time_t)(-1);
    }

    // Convert to time_t (seconds since Epoch)
    return mktime(&timeinfo);
}

int get_key(char *out) {

    FILE *f =  fopen("/dev/key_fcf8d0e08c5d621fc0dc920b97ad5079", "r");
    if (f == NULL) {                      // Check if file opened successfully
        perror("Erreur un fichier système n'existe pas : \"/dev/key_fcf8d0e08c5d621fc0dc920b97ad5079\" !\n");
        exit(-1);
    }
    int r = read(fileno(f), out, 128);
    out[128] = 0;
    fclose(f);
    if (r < 128) {
        perror("Erreur lors de la récupération de la clef dans la base de donnée. Le hash H(K) sans padding de cette clef est : fcf8d0e08c5d621fc0dc920b97ad5079.\n");
        exit(-1);
    }
    return r;
}

int get_tag(char *out) {

    FILE *f1 =  fopen("/dev/scan_detected", "r");
    if (f1 == NULL) {                      // Check if file opened successfully
        perror("Erreur un fichier système n'existe pas : \"/dev/scan_detected\" !\n");
        exit(-1);
    }
    char c[2];
    while(read(fileno(f1), &c, 1) == 0)
        sleep(1);
    fclose(f1);


    FILE *f =  fopen("/dev/scanned_tag", "r");
    if (f == NULL) {                      // Check if file opened successfully
        perror("Erreur de lecture des données de la carte RFID !\n");
        exit(-1);
    }
    int r = read(fileno(f), out, 96);
    out[96] = 0;
    fclose(f);
    return r;
}

void open_the_door() {
    FILE *f =  fopen("/dev/trigger_door", "w");
    if (f == NULL) {                      // Check if file opened successfully
        perror("Erreur un fichier système n'existe pas : \"/dev/trigger_door\" !\n");
        exit(-1);
    }
    write(fileno(f), "true", 5);
    fclose(f);
}

// return 0 if incorrect
// return 1 if correct
int authenticate(char *tag) {
    // Read the Key from DB
    char K[129];
    get_key(K);
    // Biuld blocks from tag and key
    uint8_t blocks[97];
    bzero(blocks, 97);
    int blocks_size = build_blocks(K, tag, &tag[32], blocks);
    struct md5_digest dgst;
    md5(blocks, &dgst, NULL);
    char hash_hex[33];
    hash_hex[32] = 0;
    bytes_to_hex(dgst.bytes, 16, hash_hex);
    // Verifications :
    if(strncasecmp(&tag[64], hash_hex, 32) == 0) {
        char date_bytes[11];
        date_bytes[10] = 0;
        hex_to_bytes(&tag[32], date_bytes, 10);
        time_t t = get_time_t_from_date(date_bytes);
        time_t now = time(NULL);
        if (t > now) {
            if (!strncasecmp(tag, "6a616371756573", 14))
                return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv, char **envp[]){
    char rfid_tag[97];
    bzero(rfid_tag, 97);

    char running = 1;
    while (running) {
        // Read tag from rfid scanner
        get_tag(rfid_tag);
        // Authenticate the user
        if (authenticate(rfid_tag)) {
            open_the_door();
        }
    }
    return 0;
}



