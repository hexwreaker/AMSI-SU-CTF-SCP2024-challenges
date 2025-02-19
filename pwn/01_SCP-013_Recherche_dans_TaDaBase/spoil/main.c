
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

enum CERT_LEVEL {OFFICIAL, CONFIDENTIAL, RESTRICTED, SECRET, TOP_SECRET, THAUMIEL};

struct auth_state {
    char name[64];
    time_t auth_date;
    enum CERT_LEVEL cert;
};

struct auth_state *state;

struct doc_entry {
    char name[64];
    enum CERT_LEVEL cert;
    int64_t id;
};

#define MAX_DOCUMENTS 1024
struct doc_entry *documents[MAX_DOCUMENTS];

// Print the buffer with delay between each char
// depending on speed variable.
int print_terminal(char *buff) {

    int min = 1;
    int max = 20;

    do {
        int time_ms = rand() % max + min;
        // 1000000 = 1 sec
        usleep(time_ms*600);

        putchar(*buff);
        fflush(stdout);
    }
    while (*(buff++));
    
    return 0;
}

void clear_terminal() {
    printf("\e[2J"); // Efface le terminal précédent.
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\e[H"); // Curseur en haut a gauche.
}

void suspens_points() {
    usleep(250000);
    print_terminal(".");
    usleep(250000);
    print_terminal(".");
    usleep(250000);
    print_terminal(".");
    usleep(250000);
}

int get_next_char(char *buffer, int size, char c) {
    for (int i=0; i<size; i++) {
        if (buffer[i] == c)
            return i;
    }
    return -1;
}

int replace_next_char(char *buffer, int size, char c1, char c2) {
    int next = get_next_char(buffer, size, c1);
    if (next == -1)
        return -1;
    buffer[next] = c2;
    return next;
}

char *get_command(char *cmd, int size) {
    printf("\e[34m>>>\e[0m");
    return fgets(cmd, size, stdin);
}

void print_help() {
    print_terminal("Liste des commandes de TaDaBase :\n");
    print_terminal("\t- aide : affiche l'aide de TaDaBase\n");
    print_terminal("\t- login : permet à l'utilisateur de se connecter au serveur.\n");
    print_terminal("\t- logout : permet à l'utilisateur de se déconnecter au serveur.\n");
    print_terminal("\t- infos : affiche les informations de l'utilisateur connecté.\n");
    print_terminal("\t- liste : liste les documents présents dans la base de donnée.\n");
    print_terminal("\t- inserer : insérer un document dans la base de donnée.\n");
    print_terminal("\t- supprimer : supprimer un document dans la base de donnée.\n");
    print_terminal("\t- lire : lire un document.\n");
    print_terminal("\t- quitter : quitter le client TaDaBase.\n");
}

void print_infos() {
    print_terminal("État du client :\n");
    if (state == NULL) {
        print_terminal("\t*État : \e[31mdéconnecté\e[0m."); }
    else {
        print_terminal("\t*Nom : ");
        print_terminal(state->name);
        print_terminal("\n\t*État : \e[32mconnecté\e[0m.\n");
        print_terminal("\t*Depuis le : ");
        char buff[11];
        strftime(buff, 11, "%Y-%m-%d", localtime(&state->auth_date));
        buff[10] = 0;
        print_terminal(buff);
    }
    print_terminal("\n\t*Département : Sillicium-14.\n");
    print_terminal("\t*Service : CAOE-P-06.\n");

    if (state == NULL) {
        print_terminal("\t*Niveau d'autorisation du client : \e[34mCONFIDENTIAL\e[0m\n\n");}
    else if (state->cert == CONFIDENTIAL) {
        print_terminal("\t*Niveau d'autorisation du client : \e[34mCONFIDENTIAL\e[0m\n\n");}
    else if (state->cert == RESTRICTED) {
        print_terminal("\t*Niveau d'autorisation du client : \e[34mRESTRICTED\e[0m\n\n");}
    else if (state->cert == SECRET) {
        print_terminal("\t*Niveau d'autorisation du client : \e[35mSECRET\e[0m\n\n");}
    else if (state->cert == TOP_SECRET) {
        print_terminal("\t*Niveau d'autorisation du client : \e[35mTOP SECRET\e[0m\n\n");}
    else if (state->cert == THAUMIEL) {
        print_terminal("\t*Niveau d'autorisation du client : \e[35mTHAUMIEL\e[0m\n\n");}
    else if (state->cert == OFFICIAL) {
        print_terminal("\t*Niveau d'autorisation du client : \e[34mOFFICIAL\e[0m\n\n");}
}

int login() {
    char buffer[64];
    printf("\e[34mUsername ");
    get_command(buffer, 64);
    
    state = (struct auth_state *)malloc(sizeof(struct auth_state));
    if (state == NULL) {
        return -1;
    }
    
    replace_next_char(buffer, 64, '\n', 0);
    strncpy(state->name, buffer, 64);
    state->name[63] = 0;
    state->auth_date = time(NULL);
    state->cert = CONFIDENTIAL;
    return 0;
}
int logout() {
    free(state);
    state = NULL;
    return 0;
}

int liste(){
    print_terminal("\tDoc ID | \t\t Filename \t\t | \t Accreditation level\n\e[37m");
    for (int i=0; i<MAX_DOCUMENTS; i++) {
        if (documents[i] != 0) {
            if (documents[i]->cert == CONFIDENTIAL)
                printf("\t  %d    |   %s \t |   [%s]\n", documents[i]->id, documents[i]->name, "CONFIDENTIAL");
            if (documents[i]->cert == OFFICIAL)
                printf("\t  %d    |   %s \t |   [%s]\n", documents[i]->id, documents[i]->name, "OFFICIAL");
            if (documents[i]->cert == RESTRICTED)
                printf("\t  %d    |   %s \t |   [%s]\n", documents[i]->id, documents[i]->name, "RESTRICTED");
            if (documents[i]->cert == SECRET)
                printf("\t  %d    |   %s \t |   [%s]\n", documents[i]->id, documents[i]->name, "SECRET");
            if (documents[i]->cert == TOP_SECRET)
                printf("\t  %d    |   %s \t |   [%s]\n", documents[i]->id, documents[i]->name, "TOP_SECRET");
            if (documents[i]->cert == THAUMIEL)
                printf("\t  %d    |   %s \t |   [%s]\n", documents[i]->id, documents[i]->name, "THAUMIEL");
        }
    }
    printf("\e[0m");
}

int get_offset() {
    // parcours la liste et retourne un emplacement libre
    for (int i=0; i<MAX_DOCUMENTS; i++) {
        if (documents[i] == 0)
            return i;
    }
    return -1;
}

int add_entry_raw(char *name, enum CERT_LEVEL cert) {
    int offset = get_offset();
    if (offset == -1)
        return -1;
    struct doc_entry *doc = (struct doc_entry *)malloc(sizeof(struct doc_entry));
    if (doc == NULL)
        return -1;
    strncpy(doc->name, name, 63);
    doc->name[63] = 0;
    doc->id = offset;
    doc->cert = cert;

    documents[offset] = doc;
    return 0; 
}

int add_entry() {
    int offset = get_offset();
    if (offset == -1 || state == NULL)
        return -1;
    char buffer[64];
    printf("\e[34mFilename ");
    get_command(buffer, 64);
    if(buffer[0] == '/')
        return -1;
    if(strcasestr(buffer, "..") != NULL)
        return -1;

    struct doc_entry *doc = (struct doc_entry *)malloc(sizeof(struct doc_entry));
    if (doc == NULL)
        return -1;

    replace_next_char(buffer, 64, '\n', 0);
    strncpy(doc->name, buffer, 63);
    doc->name[63] = 0;
    doc->id = offset;
    doc->cert = state->cert;

    documents[offset] = doc;
    return 0;
}

int remove_entry() {
    if (state == NULL)
        return -1;

    char buffer[64];
    printf("\e[34mDoc ID ");
    get_command(buffer, 64);
    replace_next_char(buffer, 64, '\n', 0);
    int32_t id = strtol(buffer, NULL, 10);
    if (id < 0 || id >= MAX_DOCUMENTS || documents[id] == 0 || documents[id]->cert > state->cert)
        return -1;
    free(documents[id]);
}

int read_file(char *directory, char *filename) {
    // concat directory et filename
    char buffer[1024];
    buffer[0] = 0;
    strncat(buffer, directory, 510);
    strncat(buffer, filename, 510);
    printf("Lecture du fichier : %s\n", buffer);
    // printf the file content
    FILE *f = fopen(buffer, "r");
    if (f == NULL)
        return -1;

    int32_t size = 0;
    while((size = read(fileno(f), buffer, 1024)) != 0) {
        fwrite(buffer, size, 1, stdout);
    }
    printf("\n");
    return 0;
}

int read_entry() {
    if (state == NULL)
        return -1;

    char buffer[64];
    printf("\e[34mDoc ID ");
    get_command(buffer, 64);
    replace_next_char(buffer, 64, '\n', 0);
    int32_t id = strtol(buffer, NULL, 10);
    if (id < 0 || id >= MAX_DOCUMENTS || documents[id] == 0)
        return -1;

    if (documents[id]->cert == OFFICIAL && state->cert >= OFFICIAL) {
        return read_file("./docs/official/", documents[id]->name);
    }
    else if (documents[id]->cert == CONFIDENTIAL && state->cert >= CONFIDENTIAL) {
        return read_file("./docs/confidential/", documents[id]->name);
    }
    else if (documents[id]->cert == RESTRICTED && state->cert >= RESTRICTED) {
        return read_file("./docs/restricted/", documents[id]->name);
    }
    else if (documents[id]->cert == SECRET && state->cert >= SECRET) {
        return read_file("./docs/secret/", documents[id]->name);
    }
    else if (documents[id]->cert == TOP_SECRET && state->cert >= TOP_SECRET) {
        return read_file("./docs/top_secret/", documents[id]->name);
    }
    else if (documents[id]->cert == THAUMIEL && state->cert >= THAUMIEL) {
        return read_file("./docs/thaumiel/", documents[id]->name);
    }
    else if (state->cert >= CONFIDENTIAL) {
        return read_file("./docs/autres/", documents[id]->name);
    }
    else {
        return -1;
    }
    return 0;
}

int init_files() {
    bzero(documents, sizeof(documents));
    struct dirent *entry;
    DIR *directory = opendir("./docs/official");
    if (directory != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (entry->d_type == DT_REG) {
                    add_entry_raw(entry->d_name, OFFICIAL);
                }
            }
        }
    }
    closedir(directory);
    directory = opendir("./docs/confidential");
    if (directory != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (entry->d_type == DT_REG) {
                    add_entry_raw(entry->d_name, CONFIDENTIAL);
                }
            }
        }
    }
    closedir(directory);
    directory = opendir("./docs/restricted");
    if (directory != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (entry->d_type == DT_REG) {
                    add_entry_raw(entry->d_name, RESTRICTED);
                }
            }
        }
    }
    closedir(directory);
    directory = opendir("./docs/secret");
    if (directory != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (entry->d_type == DT_REG) {
                    add_entry_raw(entry->d_name, SECRET);
                }
            }
        }
    }
    closedir(directory);
    directory = opendir("./docs/top_secret");
    if (directory != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (entry->d_type == DT_REG) {
                    add_entry_raw(entry->d_name, TOP_SECRET);
                }
            }
        }
    }
    closedir(directory);
    directory = opendir("./docs/thaumiel");
    if (directory != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (entry->d_type == DT_REG) {
                    add_entry_raw(entry->d_name, THAUMIEL);
                }
            }
        }
    }
    closedir(directory);
    return 0;
}

int lire() {
    int a = read_entry();
    if (a == -1)
        printf("\e[31m[ERREUR] : impossible de lire l'élément, il  n'existe pas ou vous n'avez pas les droits.\n\e[0m");
    return a;
}
int inserer() {
    int a = add_entry();
    if (a == -1)
        printf("\e[31m[ERREUR] : impossible d'ajouter l'élément, vous n'avez peut être pas les droits requis.\n\e[0m");
    return a;
}
int supprimer() {
    int a = remove_entry(); 
    if (a == -1)
        printf("\e[31m[ERREUR] : impossible de supprimer l'élément, il  n'existe pas ou vous n'avez pas les droits.\n\e[0m");
    return a;
}

int main(int argc, char *argv[], char **envp[]) {

    if (init_files() == -1) {
        printf("Erreur init_files !!!");
        return -1;
    }

    srand(time(NULL));
    char buffer[300];
    // Décale le terminal
    // printf("\e[%dB", 100);
    clear_terminal();
    print_terminal("\e[33m[INFO] : Appuyer sur entrée pour allumer le terminal.\e[0m");
    fgets(buffer, 2, stdin);
    clear_terminal();
    print_terminal("Exécution du processus de retour de veille");
    suspens_points();
    print_terminal("[OK]\n");
    print_terminal("[INFO] Aucune anomalie détecté.\n");
    print_terminal("Démarrage du système");
    suspens_points();
    clear_terminal();

    printf("\n");
    printf("[--------------------------------------------------------------------]\n");
    printf("[--------------------TaDaBase-1.3.0--Sillicium-14--------------------]\n");
    printf("[--------------------------------------------------------------------]\n");
    printf("\n");
    print_terminal("Bienvenue sur TaDabase, le logiciel de gestion de document de la Fondation SCP.\n");
    print_infos();
    print_terminal("[INFO] Entrez «aide» pour afficher l'aide du logiciel.\n");
    logout();

    while (get_command(buffer, 256) != NULL) {
        if (!strncasecmp(buffer, "aide", 4)) {
            print_help();
        }
        else if(!strncasecmp(buffer, "infos", 5)) {
            print_infos();
        }
        else if(!strncasecmp(buffer, "login", 5)) {
            if (!login()) {
                printf("\e[33m[INFO] «%s» vient de se connecter.\n\e[0m", state->name);
            }

        }
        else if(!strncasecmp(buffer, "logout", 5)) {
            if (state != NULL) {
                char buffer[64];
                strncpy(buffer, state->name, 63);
                buffer[63] = 0;
                logout();
                printf("\e[33m[INFO] «%s» vient de se déconnecter.\n\e[0m", buffer);
            }            
        }
        else if(!strncasecmp(buffer, "liste", 5)) {
            liste();
        }
        else if(!strncasecmp(buffer, "inserer", 5)) {
            inserer();
        }
        else if(!strncasecmp(buffer, "supprimer", 5)) {
            supprimer();
        }
        else if(!strncasecmp(buffer, "lire", 4)) {
            lire();
        }
        else if(!strncasecmp(buffer, "quitter", 4)) {
            print_terminal("Merci d'utiliser TaDaBase ! Aurevoir... ;)\n\n");
            return 0;
        }
        else {
            print_terminal("Veuillez entrer une commande correct. Voir «aide».\n");
        }
    }

    return 0;
}











