#include <FLAC/stream_encoder.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <time.h>

#include "megamix.h"

#define MAX_SINT16 32767
#define MIN_SINT16 -32768

unsigned char init_key[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void set_init_key() {
    printf("Vous : «Si seulement ce programme n'avait pas été \"stripé\" !»\n");
    
    unsigned char values[] = {9, 2, 2, 4, 6, 4, 8, 7, 9, 5, 1, 2, 3, 4};
    memcpy(&init_key[1], values, 14);
    srand(time(0));
    init_key[15] = (unsigned char)(rand()%255);
    // printf("<%02x>\n", init_key[15]);
}

int detect_debuger() {
    return ptrace(PTRACE_TRACEME, 0);
}

int __attribute__ ((constructor)) premain()
{
    if (!detect_debuger()) {
        init_key[0] = 8;
    }
    return 0;
}

// command : char command*
// cmd_bloc: char cmd_block[command_size*16]
int genere_cmd_bloc(char *command, int command_size, unsigned char *cmd_bloc) {

    // printf("genere bloc from : %s\n", command);
    printf("SCP-061 tourne la commande dans tous les sens !?\n");

    // Initalisation de la clef
    set_init_key();

    // Calcule le hash de chaque caractère
    unsigned char hash[32];
    unsigned char hashes[command_size*16];
    
    // Calcul des hash SHA-128
    for (int i=0; i < command_size; i++) {
        SHA256(&command[i], 1, hash);
        memcpy(&hashes[16*i], hash, 16);
        // printf("hash : %c -> ", command[i]);
        // for (int j=0; j<16; j++) {
        //     printf("%02x ", hashes[16*i + j]);
        // }
        // printf("\n");
    }

    // Chiffrement des blocs
    for (int i=0; i<command_size; i++) {

        unsigned char iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        AES_KEY encryptKey;
        AES_set_encrypt_key(init_key, 128, &encryptKey);
        AES_cbc_encrypt(&hashes[16*i], &cmd_bloc[16*i], 16, &encryptKey, iv, AES_ENCRYPT);
        for (int j=0; j<16; j++) {
            init_key[j] ^= cmd_bloc[16*i + j];
        }
    }

    return 0;
}

int write_pmc_music(int32_t *buffer, int samples) {
    printf("SCP-061 : «La la la lalala...»\n");

    if (samples > (int)megamix_pcm_len/2)
        samples = (int)megamix_pcm_len/2;

    // int8 array (16bit value) -> int32 array 
    for (int i=0; i<samples; i++) {
        int16_t sample = (int16_t)((megamix_pcm[2 * i + 1] << 8) | (megamix_pcm[2 * i] & 0xFF));
        buffer[i] = (int32_t)sample; // sign extend it !
    }

    return 0;
}

int mix(int32_t *buffer, int buffer_size, unsigned char *cmd_bloc, int cmd_bloc_size) {
    printf("Bon, cette fois pas de commentaire.\n");
    
    // for (int i=0; i<80; i++) {
    //     printf("%02x ", cmd_bloc[i]);
    // }

    // Integrate the cmd_bloc into the music pcm buffer.
    int prec = 0;
    for(int i=0; i<cmd_bloc_size; i++) {
        int32_t bef = buffer[prec];
        int16_t new = (int16_t)(buffer[prec]&0xff00) + (cmd_bloc[i]);
        buffer[prec] = (int32_t)new;
        // printf("\nplace %08x at %d; before : %08x ; new : %08x", buffer[prec], prec, bef, new);
        prec = prec+cmd_bloc[i];
        if (cmd_bloc[i] == 0)
            prec++;
    }

    return 0;
}

int main(int argc, char *argv[], char **envp[]) {

    if (argc != 2) {
        printf("Usage : scp-061 <commande>\n");
        exit(-1);
    }

    printf("SCP-061 commence à réfléchir... 0_0!\n");
    
    // Créer un buffer d'échantillons audio
    int frequence = 44100; // Hertz
    int duree = 30; // secondes
    uint samples = frequence * duree;


    // Créer l'encodeur FLAC
    FLAC__StreamEncoder *encoder = FLAC__stream_encoder_new();
    if(encoder == NULL) {
        printf("Euuuuhhh... Bug !\n");
        return -1;
    }

    // Configurer l'encodeur
    FLAC__bool ok = true;
    ok &= FLAC__stream_encoder_set_channels(encoder, 1);              // Stéréo
    ok &= FLAC__stream_encoder_set_sample_rate(encoder, frequence);        // kHz
    ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, 16);       // 16 bits par échantillon
    ok &= FLAC__stream_encoder_set_compression_level(encoder, 8);      // Niveau de compression moyen

    if(!ok) {
        printf("Euuuuhhh... Bug !\n");
        // printf("Erreur lors de la configuration de l'encodeur\n");
        return -1;
    }

    // Buffer d'échantillons de 16 bits (PCM)
    int32_t *buffer; 
    buffer = calloc(samples, sizeof(int32_t));

    // Génération du bloc à partir d'une commande
    unsigned char *cmd_bloc;
    int len_command = strlen(argv[1]);
    cmd_bloc = calloc(len_command*16, 1);
    if (genere_cmd_bloc(argv[1], len_command, cmd_bloc)) {
        printf("Euuuuhhh... Bug !\n");
        // printf("Erreur lors de la génération du bloc initial\n");
        return -1;
    }

    // Remplissage du buffer avec une douce musique
    write_pmc_music(buffer, samples);

    // Incorpore le bloc de commande dans le buffer final
    mix(buffer, samples, cmd_bloc, len_command*16);

    // Ouvrir le fichier de sortie
    FLAC__StreamEncoderInitStatus init_status;
    init_status = FLAC__stream_encoder_init_file(encoder, "output.audio", NULL, NULL);
    if(init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
        printf("Euuuuhhh... Bug !\n");
        // printf("Erreur lors de l'initialisation du fichier de sortie : %s\n",
            //    FLAC__StreamEncoderInitStatusString[init_status]);
        FLAC__stream_encoder_delete(encoder);
        return -1;
    }

    // Encodage des échantillons dans le fichier
    if(!FLAC__stream_encoder_process_interleaved(encoder, buffer, samples)) {
        // printf("state = %d\n", FLAC__stream_encoder_get_state(encoder));
        // printf("Erreur lors de l'encodage des échantillons audio\n");
        printf("Euuuuhhh... Bug !\n");
        FLAC__stream_encoder_delete(encoder);
        free(buffer);
        return -1;
    }

    // Finaliser l'encodage
    FLAC__stream_encoder_finish(encoder);
    FLAC__stream_encoder_delete(encoder);
    free(buffer);

    printf("SCP-061 retourne se coucher. -_-\n");
    return 0;
}

