#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <fcntl.h>


#define PORT 6000
#define MAX_BUFFER 1000
#define MAX_LENGTH_WORD 30

int fichier1;
const char *cool;

const char *readLine(FILE *file) {

    if (file == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }

    int maximumLineLength = 128;
    char *lineBuffer = (char *)malloc(sizeof(char) * maximumLineLength);

    if (lineBuffer == NULL) {
        printf("Error allocating memory for line buffer.");
        exit(1);
    }

    char ch = getc(file);
    int count = 0;

    while ((ch != '\n') && (ch != EOF)) {
        if (count == maximumLineLength) {
            maximumLineLength += 128;
            lineBuffer = realloc(lineBuffer, maximumLineLength);
            if (lineBuffer == NULL) {
                printf("Error reallocating space for line buffer.");
                exit(1);
            }
        }
        lineBuffer[count] = ch;
        count++;

        ch = getc(file);
    }

    lineBuffer[count] = '\0';
    char line[count + 1];
    strncpy(line, lineBuffer, (count + 1));
    free(lineBuffer);
    const char *constLine = line;
    return constLine;
}

int lecturelistMots(){
        if ((fichier1 = open("listeMots", O_RDONLY)) < 0) {
        printf("Erreur ouverture fichier 1\n");
        return -1;
    }
    for (int i = 0; i < sizeof(fichier1); i++){
        printf("pouet"); 
    }
}

int randomInt(int minimum_number,int max_number){
    return rand() % (max_number + 1 - minimum_number) + minimum_number;
}

int ouvrirUneSocketAttente() {
    int socketTemp;
    int longueurAdresse;
    struct sockaddr_in coordonneesServeur;

    socketTemp = socket(PF_INET, SOCK_STREAM, 0); //Création d'une socket avec paramètre PF_INET : domaine IPV4 pour serveur, SOCK_STREAM : TCP, 0 : protocole

    if (socketTemp < 0) {//Détection erreur création socket
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare l’adresse d’attachement locale
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse); //memset pour initialiser la zone mémoire avec une valeur, ici 0x00

    coordonneesServeur.sin_family = PF_INET; //set up la famille d'adresse à PF_INET (similaire à AF_INET)
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_addr.s_addr = htonl(INADDR_ANY); //Création de l'adresse IPV4
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT); //Affection au port 6000

    if (bind(socketTemp, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("erreur de bind\n"); //Erreur de liaison entre la socket et le serveur
        exit(EXIT_FAILURE);
    }

    if (listen(socketTemp, 5) == -1) {
        printf("erreur de listen\n"); //Erreur de mise en attente de la socket
        exit(EXIT_FAILURE);
    }

    printf("En attente de connexion...\n");

    return socketTemp;
}


int main(int argc, char const *argv[]) {
    int fdSocketAttente;
    int fdSocketCommunication;
    struct sockaddr_in coordonneesAppelant;
    char tampon[MAX_BUFFER];
    int nbRecu;
    const char * tablo = readLine(fichier1);

    fdSocketAttente = ouvrirUneSocketAttente();

    socklen_t tailleCoord = sizeof(coordonneesAppelant);

    if ((fdSocketCommunication = accept(fdSocketAttente, (struct sockaddr *) &coordonneesAppelant,
                                        &tailleCoord)) == -1) {
        printf("erreur de accept\n"); //Erreur de connexion
        printf(tablo[randomInt(1,10)]);
        exit(-1);
    }

    printf("Client connecté\n");

    // on attend le message du client
    // la fonction recv est bloquante
    nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0); // on attend la réponse du client

    if (nbRecu > 0) { //Si on reçoit la réponse du client
        tampon[nbRecu] = 0;
        printf("Recu : %s\n", tampon); //On affiche le message du client
        printf(tablo[randomInt(1,10)]);
    }

    printf("Envoi du message au client.\n");
    strcpy(tampon, "Message renvoyé par le serveur vers le client !"); //concaténation d'une chaine de caractère
    // on envoie le message au client
    send(fdSocketCommunication, tampon, strlen(tampon), 0);  //Envoi d'un bloc de donnée sur la socket

    close(fdSocketCommunication); //Cloture le canal de communication, fermeture de la connexion TCP
    close(fdSocketAttente);

    return EXIT_SUCCESS;
}
