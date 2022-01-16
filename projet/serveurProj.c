#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define PORT 6000
#define MAX_BUFFER 1000
#define MAX_CLIENTS 10

const char *EXIT = "exit";

int rndmEntier = 0;
char mot[200] = "";
char motCacher[200] = "";
int nbMots = 0;

//tirage aléatoire d'un entier entre deux bornes
int randomInt(int borninf, int bornsup)
{
    int n;
    srand((unsigned)time(NULL));
    n = (rand() % (bornsup - borninf + 1)) + borninf;
    return n;
}

//sélectionne un mot aléatoire dans la liste
void wordPicker()
{
    memset(mot, 0, strlen(mot));
    FILE *fichier = fopen("listeMots", "r");
    if (fichier == NULL)
    {
        printf("Cannot open file\n");
        exit(0);
    }
    while (fgets(mot, 30, fichier) != NULL)
    {
        nbMots++;
    }

    rndmEntier = randomInt(1, nbMots);
    rewind(fichier);
    while (fgets(mot, 30, fichier) != NULL && nbMots != rndmEntier)
    {
        nbMots--;
    }

    fclose(fichier);
}

//créé une version cachée du mot
void hiddenWord(char *word)
{
    memset(motCacher, 0, strlen(motCacher));
    int taille = strlen(word);
    for (int i = 0; i < taille - 1; i++)
    {
        strcat(motCacher, "-");
    }
    strcat(motCacher, "\n");
}

int main(int argc, char const *argv[])
{
    int fdSocketAttente;
    int fdSocketCommunication;
    struct sockaddr_in coordonneesServeur;
    struct sockaddr_in coordonneesAppelant;
    char tampon[MAX_BUFFER];
    int nbRecu;
    int longueurAdresse;
    int pid;

    fdSocketAttente = socket(PF_INET, SOCK_STREAM, 0);

    if (fdSocketAttente < 0)
    {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare l’adresse d’attachement locale
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (bind(fdSocketAttente, (struct sockaddr *)&coordonneesServeur, sizeof(coordonneesServeur)) == -1)
    {
        printf("erreur de bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(fdSocketAttente, 5) == -1)
    {
        printf("erreur de listen\n");
        exit(EXIT_FAILURE);
    }

    socklen_t tailleCoord = sizeof(coordonneesAppelant);

    int nbClients = 0;
    //test pour ne pas dépasser le nb max de clients
    while (nbClients < MAX_CLIENTS)
    {
        if ((fdSocketCommunication = accept(fdSocketAttente, (struct sockaddr *)&coordonneesAppelant,
                                            &tailleCoord)) == -1)
        {
            printf("erreur de accept\n");
            exit(EXIT_FAILURE);
        }

        printf("Client connecté - %s:%d\n",
               inet_ntoa(coordonneesAppelant.sin_addr),
               ntohs(coordonneesAppelant.sin_port));

        if ((pid = fork()) == 0)
        {
            close(fdSocketAttente);

            while (1)
            {
                wordPicker();
                hiddenWord(mot);
                send(fdSocketCommunication, mot, strlen(mot), 0);
                send(fdSocketCommunication, motCacher, strlen(motCacher), 0);
                
                // on attend le pseudo du client
                // la fonction recv est bloquante
                nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0);

                if (nbRecu > 0)
                {
                    tampon[nbRecu] = 0;
                    printf("Pseudo de %s:%d : %s\n",
                           inet_ntoa(coordonneesAppelant.sin_addr),
                           ntohs(coordonneesAppelant.sin_port),
                           tampon);
                }
                //réception du score
                nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0);

                if (nbRecu > 0)
                {
                    tampon[nbRecu] = 0;
                    printf("Score de %s:%d : %s\n",
                           inet_ntoa(coordonneesAppelant.sin_addr),
                           ntohs(coordonneesAppelant.sin_port),
                           tampon);
                }

                //est ce que le joueur rejoue ?
                nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0);
                if (nbRecu > 0)
                {
                    tampon[nbRecu] = 0;
                    printf("Joueur %s:%d :rejoue t'il ? %s\n",
                           inet_ntoa(coordonneesAppelant.sin_addr),
                           ntohs(coordonneesAppelant.sin_port),
                           tampon);
                }

                send(fdSocketCommunication, tampon, strlen(tampon), 0);
            }

            exit(EXIT_SUCCESS);
        }

        nbClients++;
    }

    //on ferme les sockets
    close(fdSocketCommunication);
    close(fdSocketAttente);

    //attente de tout les clients
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        wait(NULL);
    }

    printf("Fin du programme.\n");
    return EXIT_SUCCESS;
}
