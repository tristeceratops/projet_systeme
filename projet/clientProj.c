#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>


#define MAXERREUR 6
#define PORT 6000
#define MAX_BUFFER 1000

const char *EXIT = "exit";





char mot[200] = "";
int nbMots = 0;
int rndmEntier = 0;
char buffer[200] = "";
char motCacher[200] = "";
int score;
char lettre;
char tamponScore[200];
char tamponRejouer[200];
char valueReplay;


void afficherAscii(char * pathname){
    FILE * fichier = fopen(pathname,"r");
    if ( fichier == NULL ) {
        printf("Cannot open file\n");
        exit( 0 );
    }
    while (fgets(buffer,30,fichier) != NULL)
    {
        printf("%s",buffer);
    }
    fclose(fichier);
}

void hiddenWord(char * word){
    int taille = strlen(word);
    for (int i = 0; i<taille-1;i++)
    {
        strcat(motCacher,"-");
    }
    strcat(motCacher,"\n");
}



int verifLettre(char lettre){
    int taille = strlen(mot);
    int true = -1;
    for (int i = 0; i<taille-1;i++)
    {
        if(mot[i] == lettre)
        {
            motCacher[i] = lettre;
            true = 0;
        }
    }
    return true;
}

int verifMot(char * word){
    int taille = strlen(word);
    for (int i = 0;i<taille-1;i++){
        if (strcmp(motCacher,mot)!=0)
        {
            return 0;
        }
    }
    return 1;
}

void demanderLettre(){
    printf("Saisissez une lettre\n");
    scanf(" %c",&lettre);

}

void game(){
    system("clear");
    // wordPicker();
    // hiddenWord(mot);
    int erreur = 0;
    int trouve = 0;
    char dessin[200]= "ascii/ascii";
    char erreurStr[2] = "";

    do
    {
        if(erreur>0){
            sprintf(erreurStr,"%d",erreur);
            strcat(dessin,erreurStr);
            strcat(dessin,".txt");
            afficherAscii(dessin);
            memset(dessin,0,strlen(dessin));
            strcat(dessin,"ascii/ascii");
        }
        printf("%s",motCacher);
        demanderLettre();
        if(verifLettre(lettre)<0)
        {
            erreur++;
        }
        trouve = verifMot(mot);
        system("clear");
    } while (erreur <= MAXERREUR && trouve == 0);
    score = 1000 * (7-erreur);
    if(trouve == 0)
    {
        afficherAscii("ascii/asciiBad.txt");
        printf("\nDommage !!! Le mot était : %s\n",mot);

    }
    else{
        afficherAscii("ascii/asciiGood.txt");
        printf("\nBien joué ! Tu as réussi à trouver le mot : %s\nTon score est de : %d\n",mot,score);
        sprintf(tamponScore,"%d",score);
        strtok(tamponScore, "\n");

    }
}

void lireMessage(char tampon[]) {
    printf("Saisir votre pseudo :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}
void replay(){
    do {
        printf("Voulez vous rejouer ? oui(y)/non(n)\n");
        scanf(" %c",&valueReplay);
        valueReplay=tolower(valueReplay);
    }while(valueReplay!='y'&&valueReplay!='n');
}

int main(int argc , char const *argv[]) {
    int fdSocket;
    int nbRecu;
    struct sockaddr_in coordonneesServeur;
    int longueurAdresse;
    char tampon[MAX_BUFFER];

    fdSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (fdSocket < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare les coordonnées du serveur
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // adresse du serveur
    inet_aton("127.0.0.1", &coordonneesServeur.sin_addr);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (connect(fdSocket, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("connexion impossible\n");
        exit(EXIT_FAILURE);
    }

    printf("connexion ok\n");




    while (1) {
        nbRecu = recv(fdSocket, mot, MAX_BUFFER, 0);

        if (nbRecu > 0) {
            mot[nbRecu] = 0;
            printf("La partie va commencer\n");

        }

        nbRecu = recv(fdSocket, motCacher, MAX_BUFFER, 0);

        if (nbRecu > 0) {
            motCacher[nbRecu] = 0;
            printf("Recu : %s\n", motCacher);

        }

        lireMessage(tampon);

        if (testQuitter(tampon)) {

            send(fdSocket,tampon, strlen(tampon), 0);
            break; // on quitte la boucle
        }

        // on envoie le message au serveur
        send(fdSocket, tampon, strlen(tampon), 0);
        game();
        send(fdSocket,tamponScore, strlen(tamponScore),0);
        replay();
        memset(tamponRejouer,0, strlen(tamponRejouer));
        strcat(tamponRejouer,&valueReplay);
        printf("%s\n",tamponRejouer);
        send(fdSocket,tamponRejouer, strlen(tamponRejouer),0);
        if(valueReplay=='n'){
            break;
        }

//        // on attend la réponse du serveur
//        nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);
//
//        if (nbRecu > 0) {
//            tampon[nbRecu] = 0;
//            printf("Recu : %s\n", tampon);
//
//            if (testQuitter(tampon)) {
//                break; // on quitte la boucle
//            }
//        }
    }

    close(fdSocket);

    return EXIT_SUCCESS;
}




