#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6000 //On définit le port à 6000
#define MAX_BUFFER 1000


int ouvrirUneConnexionTcp() {
    int socketTemp;
    int longueurAdresse;
    struct sockaddr_in coordonneesServeur;

    socketTemp = socket(AF_INET, SOCK_STREAM, 0); //Création d'une socket avec paramètre AF_INET : domaine IPV4 pour client, SOCK_STREAM : TCP, 0 : protocole

    if (socket < 0) { //Détection erreur création socket
        printf("socket incorrect\n");
        exit(EXIT_FAILURE);
    }

    // On prépare les coordonnées du serveur
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse); //memset pour initialiser la zone mémoire avec une valeur, ici 0x00

    coordonneesServeur.sin_family = PF_INET; //set up la famille d'adresse à PF_INET (similaire à AF_INET)
    // adresse du serveur
    inet_aton("127.0.0.1", &coordonneesServeur.sin_addr); //Inscrit l'adresse dans l'ordre réseau
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT); //Affection au port 6000

    if (connect(socketTemp, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) { //Vérification de la connexion
        printf("connexion impossible\n");
        exit(EXIT_FAILURE);
    }

    printf("connexion ok\n");

    return socketTemp;
}


int main(int argc, char const *argv[]) {
    int fdSocket;
    int verifConServ;
    char tampon[MAX_BUFFER];
    char lettre[MAX_BUFFER];
    char choix[MAX_BUFFER];
    char resultat[MAX_BUFFER];

    fdSocket = ouvrirUneConnexionTcp();
    if (fdSocket<0){return -1}
    printf("Connexion au serveur...\n");


    verifConServ = recv(fdSocket, tampon, MAX_BUFFER, 0); // on attend la réponse du serveur

    if (verifConServ < 0) { //Si on ne recoit pas la reponse du serveur
        perror("connexion impossible");
        exit(EXIT_FAILURE);
        return -1;
    }
    if (verifConServ > 0) { //Si on reçoit la réponse du serveur
        tampon[verifConServ] = 0;
        printf("\nLe Jeu peut maintentant commencer !%s !\n",tampon); //On affiche le message de debut de partie.
    }

    while (1){
        if(read(fdSocket ,resultat,MAX_BUFFER)<0){
            perror("Socket Erreur");
            exit(EXIT_FAILURE);
            return -1;
        }

        if ((strstr(resultat, "Gagne") != NULL) || (strstr(resultat, "Perdu") != NULL))
        {

            printf("%s\n", resultat);
            printf("\n\nOn recommence ? Oui(1) ou Non(0)\n");
            scanf("%d",&continuer);
            sprintf(choix,"%d",continuer);

            if(continuer==1)
            {

                if (write(sock, choix, strlen(choix)+1) < 0)
                {
                    perror("Socket Erreur 2");
                    exit(EXIT_FAILURE);
                    return -1;
                }

                if (read(sock, message, BUFFER_SIZE) < 0)
                {
                    perror("Socket Erreur 3");
                    exit(EXIT_FAILURE);
                    return -1;
                }

            }
            else{
                printf("La partie est finie , vous quittez le jeu.")
                close(fdSocket); //Cloture le canal de communication, fermeture de la connexion TCP
                break;
            }
        }
    //    strcpy(tampon, "Message du client vers le serveur"); //concaténation d'une chaine de caractère
    //    send(fdSocket, tampon, strlen(tampon), 0); //Envoi d'un bloc de donnée sur la socket


    }

    return EXIT_SUCCESS;
}
