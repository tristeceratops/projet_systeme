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
        printf("socket incorrecte\n");
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
    int nbRecu;
    char tampon[MAX_BUFFER];

    fdSocket = ouvrirUneConnexionTcp();

    printf("Envoi du message au serveur.\n");
    strcpy(tampon, "Message du client vers le serveur"); //concaténation d'une chaine de caractère
    send(fdSocket, tampon, strlen(tampon), 0); //Envoi d'un bloc de donnée sur la socket

    nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0); // on attend la réponse du serveur

    if (nbRecu > 0) { //Si on reçoit la réponse du serveur
        tampon[nbRecu] = 0;
        printf("Recu : %s\n", tampon); //On affiche le message du serveur
    }

    close(fdSocket); //Cloture le canal de communication, fermeture de la connexion TCP

    return EXIT_SUCCESS;
}
