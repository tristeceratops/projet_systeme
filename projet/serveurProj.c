#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

char chaine[200] = "";
int nbMots = 0;
int rndmEntier = 0;


int randomInt(int borninf,int bornsup)
{
    int n;
    srand((unsigned) time(NULL)); /* MODIF */
    n = (rand() % (bornsup - borninf +1)) + borninf;
    return n;
}

char * wordPicker(){
    FILE * fichier = fopen("listeMots","r");
     if ( fichier == NULL ) {
        printf( "Cannot open file %s\n");
        exit( 0 );
    }
    while (fgets(chaine,30,fichier) != NULL)
    {
        nbMots ++;
    }
    
    rndmEntier =  randomInt(1,nbMots);
    rewind(fichier);
    while (fgets(chaine,30,fichier) != NULL && nbMots != rndmEntier)
    {
            nbMots --;
    }
    
    fclose(fichier);
    return chaine;
}






int main(int argc, char const *argv[]) {
    char * chariot;
   chariot= wordPicker();
   printf(chariot);
    return EXIT_SUCCESS;
}
