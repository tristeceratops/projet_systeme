#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define MAXERREUR 6

char mot[200] = "";
int nbMots = 0;
int rndmEntier = 0;
char buffer[200] = "";
char motCacher[200] = "";
int score;
char lettre;




int randomInt(int borninf,int bornsup)
{
    int n;
    srand((unsigned) time(NULL)); /* MODIF */
    n = (rand() % (bornsup - borninf +1)) + borninf;
    return n;
}

void wordPicker(){
    FILE * fichier = fopen("listeMots","r");
     if ( fichier == NULL ) {
        printf("Cannot open file\n");
        exit( 0 );
    }
    while (fgets(mot,30,fichier) != NULL)
    {
        nbMots ++;
    }
    
    rndmEntier =  randomInt(1,nbMots);
    rewind(fichier);
    while (fgets(mot,30,fichier) != NULL && nbMots != rndmEntier)
    {
            nbMots --;
    }
    
    fclose(fichier);
}

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
    wordPicker();
    hiddenWord(mot);
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
        afficherAscii("asciiBad.txt");
        printf("\nDommage !!! Le mot était : %s\n",mot);
        
    }
    else{
        afficherAscii("asciiGood.txt");
        printf("\nBien joué ! Tu as réussi à trouver le mot : %s\nTon score est de : %d\n",mot,score);
    }
}


int main(int argc, char const *argv[]) {
   game();
   return EXIT_SUCCESS;
}
