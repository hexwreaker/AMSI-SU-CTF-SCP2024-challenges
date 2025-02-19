


# L'objectif est de lire les fichiers de niveau «SECRET».

# Deux affaiblissements sont implémentés :

# La première concerne la fonction "login", ici l'utilisateur peut se connecter plusieurs fois.
# 
#   fonction originale :
# int login() {
#     state = (struct auth_state *)malloc(sizeof(struct auth_state));
#     if (state == NULL) {
#         return -1;
#     }
#     memcpy(state->name, "jacques", 7);
#     state->name[7] = 0;
#     state->auth_date = time(NULL);
#     state->cert = CONFIDENTIAL;
#     return 0;
# } 
# 
#   fonction originale :
# int login() {
#     if (state != NULL)
#         return -1;
#     state = (struct auth_state *)malloc(sizeof(struct auth_state));
#     if (state == NULL) {
#         return -1;
#     }
#     memcpy(state->name, "jacques", 7);
#     state->name[7] = 0;
#     state->auth_date = time(NULL);
#     state->cert = CONFIDENTIAL;
#     return 0;
# } 

# La seconde concerne la fonction "remove_entry", ici la référence d'une note supprimée est gardée.
# 
#   fonction originale :
# int remove_entry() {
#     if (state == NULL)
#         return -1;
#     char buffer[64];
#     printf("\e[34mDoc ID ");
#     get_command(buffer, 64);
#     replace_next_char(buffer, 64, '\n', 0);
#     int32_t id = strtol(buffer, NULL, 10);
#     if (id < 0 || id >= MAX_DOCUMENTS || documents[id] == 0 || documents[id]->cert > state->cert)
#         return -1;
#     free(documents[id]);
#     documents[id] = 0;   
# }
#
#   fonction affaiblie :
# int remove_entry() {
#     if (state == NULL)
#         return -1;
#     char buffer[64];
#     printf("\e[34mDoc ID ");
#     get_command(buffer, 64);
#     replace_next_char(buffer, 64, '\n', 0);
#     int32_t id = strtol(buffer, NULL, 10);
#     if (id < 0 || id >= MAX_DOCUMENTS || documents[id] == 0 || documents[id]->cert > state->cert)
#         return -1;
#     free(documents[id]);
# }


# Le principe de l'attaque est de lire un document dont les informations ont été écrasés par "l'état" de conncetion de l'utilisteur.
# on indique le nom du fichier que l'on veut lire en tant que nom d'utilisateur. 

# Actions :
#       - login bla
#       - inserer bla
#       - supprimer 5
#       - login ../secret/secteurB7_credentials.txt
#       - lire 5




