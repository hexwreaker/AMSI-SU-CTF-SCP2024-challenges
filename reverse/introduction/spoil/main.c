#include <stdio.h>
#include <unistd.h>

int main() {

    char key[3];

    printf("Récupération de la clef...\n");
    fflush(stdout);
    printf("Veuillez entrer la clef secrète >>>");
    fflush(stdout);
    fgets(key, 3, stdin);
    printf("\nClef récupérée !\n");
    fflush(stdout);

    

    // AMSI{L3sC4fard5S0n7Tr0pM1gn0n5!}
    char secret[33] = {0x16,0x6a,0x04,0x6e,0x2c,0x6b,0x64,0x54,0x14,0x13,0x31,0x46,0x25,0x43,0x62,0x74,0x67,0x49,0x60,0x73,0x25,0x17,0x27,0x6a,0x66,0x40,0x39,0x17,0x39,0x12,0x76,0x5a};
    secret[32] = 0;
    printf("Déchiffrement en cours");
    fflush(stdout);

    for (int i=0; i<16; i++) {
        printf(".");
        fflush(stdout);
        sleep(3);
        secret[i*2] ^= key[0];
        secret[i*2+1] ^= key[1];
        printf(".");
        fflush(stdout);
        sleep(3);
    }

    printf("[OK]\n");
    fflush(stdout);

    printf("\nVotre secret est : %s  !\n", secret);
    fflush(stdout);

    return 0;
}