#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define MAXERREUR 7

char mot[200] = "";
int nbMots = 0;
int rndmEntier = 0;
char buffer[200] = "";
char motCacher[200] = "";
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
        printf( "Cannot open file %s\n");
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
        printf( "Cannot open file %s\n");
        exit( 0 );
    }
    while (fgets(buffer,30,fichier) != NULL)
    {
        printf(buffer);
    }
    fclose(fichier);
}

void hiddenWord(char * word){
    int taille = strlen(word);
    for (int i = 1; i<taille;i++)
    {
        strcat(motCacher,"-");
    }
    strcat(motCacher,"\n");
}



void verifLettre(char lettre){
    int taille = strlen(mot);
    for (int i = 1; i<taille;i++)
    {
        if(mot[i-1] == lettre)
        {
            motCacher[i-1] == lettre;
        }
    }
    
}

void demanderLettre(){
    printf("Saisissez une lettre\n");
    scanf(" %c",lettre);
    printf("%c",lettre);
    verifLettre(lettre);
}

void game(){
    wordPicker();
    hiddenWord(mot);
    int erreur = 0;
    int trouve = 0;
    do 
    {
        printf(motCacher);
        printf("%c",mot[0]);
        demanderLettre();
    } while (erreur <= MAXERREUR && trouve == 0);
}


int main(int argc, char const *argv[]) {
  game();
   return EXIT_SUCCESS;
}
