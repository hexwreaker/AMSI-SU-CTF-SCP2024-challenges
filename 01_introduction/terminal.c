#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <strings.h>

// Print the buffer with delay between each char
// depending on speed variable.
int print_terminal(char *buff) {

    int min = 1;
    int max = 20;

    do {
        int time_ms = rand() % max + min;
        // 1000000 = 1 sec
        usleep(time_ms*1000);

        putchar(*buff);
        fflush(stdout);
    }
    while (*(buff++));
    
    return 0;
}


int main(int argc, char *argv[], char **envp[]) {

    srand(time(NULL));
    char buffer[300];
    // Décale le terminale
    // printf("\e[%dB", 100);
    printf("\e[2J"); // Efface le terminal précédent.
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\e[H"); // Curseur en haut a gauche.
    print_terminal("\e[33m[INFO] : Appuyer sur entrée pour passer au dialogue suivant.\e[0m\n\n");
    

    print_terminal("\e[34mAu centre de la pièce se trouve un bureau et trois chaises. Les deux agents s'assoient et vous invitent à faire de même.\e[0m\n\n");
    print_terminal("\e[34mL'agent A prend la parole.\e[0m\n\n- Bienvenue dans les quartiers administratifs de la \"Fondation\" je suis l'agent Alice et voici l'agent Bob.\n");
    fgets(buffer, 2, stdin);

    print_terminal("\e[34mL'Agent Bob, hoche la tête en signe de bienvenue.\e[0m\n\n- J'espère que vous avez fait bon voyage depuis la-haut, comme vous l'aurez compris la Fondation emploie les meilleurs moyens possibles afin de garantir ses exigences en matière de sécurité.\n");
    fgets(buffer, 2, stdin);


    print_terminal("\e[34mElle pose un dossier sur la table, qui porte votre nom, puis se lance dans un monologue explicatif :\e[0m\n\n- Vous êtes ici parce que vous avez répondu à l'appel de l'humanité, comme toutes les personnes ici qui oeuvre pour la protéger de nombreux dangers. L'histoire de la Fondation SCP est longue et personne ne la connaît entièrement. Nous ne voyons qu'une partie d'elle car nous suivons le principe de \"Nécessité d\'informations\".");
    print_terminal("\nLe monde a ses limites que nous ignorons, et les anomalies qui s\'y trouvent se sont avérés mortels par le passé. L\'humanité a dépassé cette époque et pourtant les anomalies sont toujours présentes et de nouvelles sont découvertes chaque jour.\n");
    fgets(buffer, 2, stdin);


    print_terminal("\e[34mElle marque une pause et poursuit ses explications :\e[0m\n\n");

    print_terminal("- La Fondation a été créer pour répondre à trois objectifs :\n\t- Sécuriser\n\t- Contenir\n\t- Protéger\n\n");
    sleep(1);

    print_terminal("Pour remplir ses missions la Fondation n'a pas de limite légale et possèdent des moyens humains, technologiques et financiers quasi-illimités.\nAvant d'aller plus loin vous devez comprendre que les règles de sécurité ne doivent JAMAIS être enfreintes et que la Fondation s'arroge le droit de remédier au");
    usleep(500000);
    putchar('.');
    fflush(stdout);
    usleep(500000);
    putchar('.');
    fflush(stdout);
    usleep(500000);
    putchar('.');
    fflush(stdout);
    print_terminal(" \"problème\" par tout les moyens possibles.\n");
    fgets(buffer, 2, stdin);

    print_terminal("\e[34mElle ouvre le dossier, consulte les premières pages et reprend :\e[0m\n\n");
    sleep(1);

    print_terminal("- Vos résultats aux tests vous donne accès au poste d'agent SCP de classe C. Vos compétences dans le domaine des sciences informatique sont précieuses et nous aurons besoins de vous sur de nombreux théâtres d'opération. Vous allez intégrez la Cyber Task Force que je commande, je serai votre référente direct et vous enverrai vos ordres de missions.\nBien, cela étant dit l'agent Bob va effectuer un dernier test. Nous nous retrouverons plus tard.\n");
    fgets(buffer, 2, stdin);

    print_terminal("\e[34mL'agent Bob remercie votre référente et prend la parole à son tour :\e[0m\n\n- Permettez moi de m'assurer une dernière fois, de la qualité de vos compétences et de vos connaissances avec ces quelques questions :\n");
    fgets(buffer, 2, stdin);

    // Questions
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\e[H"); // Curseur en haut a gauche.
    print_terminal("\e[33m[INFO] : Les réponses sont insensibles à la casse.\e[0m\n\n");
    // Question 1 :
    print_terminal("\e[35mQuestion #1 :\e[0m Quel est le nom d'agent de votre référente ?\n> ");
    fgets(buffer, 256, stdin);
    if (!strncasecmp(buffer, "alice", 5)) {
        print_terminal("\n- C'est juste.\n\n");
        // Question 2 :
        print_terminal("\e[35mQuestion #2 :\e[0m Quand les règles de sécurité doivent être enfreintes ?\n> ");
        fgets(buffer, 256, stdin);
        if (!strncasecmp(buffer, "jamais", 6)) {
            print_terminal("\n- Correct.\n\n");
            // Question 3 :
            print_terminal("\e[35mQuestion #3 :\e[0m A quel étage nous trouvons nous ? (en base 10)\n> ");
            fgets(buffer, 256, stdin);
            if (!strncasecmp(buffer, "-111", 4)) {
                print_terminal("\n- Bravo c'est exact. Voici votre code d'accès : AMSI{S3c0uR1r_c0NT3n1R_Pr0teg3R}\n\n");
                fgets(buffer, 256, stdin);
                return 0;
            }
        }
    }
    
    print_terminal("\n\n\e[31mUne alarme retentit ! La salle se trouve tout à coup baignée dans une lumière rouge et la porte s'ouvre d'où rentre une bonne vingtaine gardes ! Ils semblent en avoir après vous...\e[0m");
    print_terminal("\n\nVous avez sûrement rater quelque chose");
    usleep(500000);
    putchar('.');
    fflush(stdout);
    usleep(500000);
    putchar('.');
    fflush(stdout);
    usleep(500000);
    putchar('.');
    fflush(stdout);

    return 0;
}









