#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>

#if defined (linux)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>

    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;
#elif defined (WIN32) || defined (_WIN32)
    #include <winsock2.h>
    typedef int socklen_t;
#endif

/**
* @author Joey
* @name Morpion Server
*/
int main(int argc, char* argv[])
{
    #ifdef WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 0), &wsa);
    #endif // WIN32
    /// Initialisation Variable
    int const SIZE_MSG = 10;
    char answer[SIZE_MSG];
    /** SOCKET = int (Linux). Socket d'écoute*/
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    /// Initialisation adresse Local
    SOCKADDR_IN adrLocal; // on crée la structure
    //memset ((char*) &adrLocal, '\0', sizeof(SOCKADDR_IN));
    adrLocal.sin_family = AF_INET;
    adrLocal.sin_port = htons(atoi(argv[1])); // Serveur : Définir le port obligatoirement !
    adrLocal.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("%d \n",bind(sock, (SOCKADDR*) &adrLocal, sizeof(adrLocal)));

    /** SOCKADDR_IN = int (linux). Adresse des joueurs*/
    SOCKADDR_IN adrJoueurA;
    SOCKADDR_IN adrJoueurB;

    printf("Démarrage du serveur.\n");
    if(listen(sock, 2) == 0)
    {
        /** SOCKADDR_IN = int (linux). Socket des joueurs*/
        SOCKET sockJoueurA;
        SOCKET sockJoueurB;

        printf("Le serveur est maintenant connecté.\n");

        while(true)
        {
            /** socklen_t = int (Windows). Taille des messages*/
            socklen_t sizeMsg = SIZE_MSG;


            printf("En attente de nouveau joueurs (0/2)\n");
            sockJoueurA = accept(sock, (SOCKADDR*) &adrJoueurA, &sizeMsg);
            printf("En attente de nouveau joueurs (1/2)\n");
            sockJoueurB = accept(sock, (SOCKADDR*) &adrJoueurB, &sizeMsg);
            printf("En attente de nouveau joueurs (2/2)\n");
            //int grid[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
            char grid[SIZE_MSG] = "123456789";
            bool j1 = true;
            bool jeuxOk = true;
            bool partieTerminer = false;

            //int gridVerif[3][3];
            char gridVerif[SIZE_MSG];

            char msgVerif[SIZE_MSG];
            int oldValue = 0;

            printf("La partie commence.\n");
            while(!partieTerminer)
            {
                /*if(jeuxOk)
                {
                    memcpy(msgVerif, grid, SIZE_MSG);
                    jeuxOk = false;
                }*/
                /// Prévient le joueur
                send(j1? sockJoueurA:sockJoueurB, jeuxOk? gridVerif:msgVerif, SIZE_MSG, 0);
                //send(j1? sockJoueurA:sockJoueurB, msgVerif, SIZE_MSG, 0);
                /// attente réponse
                recv(j1? sockJoueurA:sockJoueurB, answer, SIZE_MSG, 0);
                //memcpy(gridOrMsg, grid, SIZE_MSG);
                ///jouer
                //printf("Valeur recu : '%s'.\n", answer);

               /* if(strcmp(answer, "QUIT")  == 0 || strcmp(answer, "quit")  == 0)
                {
                    partieTerminer = true;
                }
                else if((strcmp(answer, "ACP")  == 0 || strcmp(answer, "acp")  == 0) && oldValue != 0)
                {
                    j1 = !j1;
                    oldValue = 0;
                    memcpy(grid, gridVerif, SIZE_MSG);
                    jeuxOk = true;
                }
                else
                {
                    int value = atoi(answer);
                    if(value > 0 && value < 10)
                    { // Si la case peu être jouée
                        memcpy(gridVerif, grid, SIZE_MSG);
                        int x = value%3;
                        int y = (value-x)/3;

                        //if(gridVerif[value - 1] == 'X' || gridVerif[value - 1] == 'O')
                        if(gridVerif[x][y] == 'X' || gridVerif[x][y] == 'O')
                        {
                            sprintf(msgVerif, "EAP"); // Error Already Play
                            jeuxOk = false;
                        }
                        else
                        {
                            //gridVerif[value - 1] = j1? 'X':'O';
                            gridVerif[x][y] = j1? 'X':'O';
                            oldValue = value;
                            jeuxOk = true;
                        }
                    }
                    else
                    { // si elle ne peu pas être jouer.
                        sprintf(msgVerif, "EWV"); // Error Wrong Value
                        jeuxOk = false;
                    }

                }*/

                int value = atoi(answer);
                if(value == 0)
                { /// Si la chaine ne peu être convertie
                    if(strcmp(answer, "QUIT")  == 0 || strcmp(answer, "quit")  == 0)
                    {
                        partieTerminer = true;
                    }
                    else if((strcmp(answer, "ACP")  == 0 || strcmp(answer, "acp")  == 0) && oldValue != 0)
                    {
                        j1 = !j1;
                        oldValue = 0;
                        memcpy(grid, gridVerif, SIZE_MSG);
                        jeuxOk = true;
                    }
                    else
                    {
                        sprintf(msgVerif, "ewv"); // Error Wrong Value
                        jeuxOk = false;
                    }
                }
                else
                { /// Si la chaine peu être convertie
                    //printf("Valeur convertie : '%d'.\n", value);
                    if(value > 0 && value < 10)
                    { // Si la case peu être jouée
                        memcpy(gridVerif, grid, SIZE_MSG);

                        int x = value%3;
                        int y = (value-x)/3;

                        if(gridVerif[value - 1] == 'X' || gridVerif[value - 1] == 'O')
                        //if(gridVerif[x][y] == 'X' || gridVerif[x][y] == 'O')
                        {
                            sprintf(msgVerif, "eap"); // Error Already Play
                            jeuxOk = false;
                        }
                        else
                        {
                            gridVerif[value - 1] = j1? 'X':'O';
                            //gridVerif[x][y] = j1? 'X':'O';
                            oldValue = value;
                            jeuxOk = true;
                        }
                    }
                    else
                    { // si elle ne peu pas être jouer.
                        sprintf(msgVerif, "ewv"); // Error Wrong Value
                        jeuxOk = false;
                    }

                } // fin tour

                if(partieTerminer)
                {
                    sprintf(msgVerif, "quit");
                    send(sockJoueurA, msgVerif, SIZE_MSG, 0);
                    send(sockJoueurB, msgVerif, SIZE_MSG, 0);
                }

            }
            printf("La partie est terminé.\n");

        }
        shutdown(sock, 0);
    }
    else
    {
        printf("Problème lors de la création de la file d'attente\n");
        return -1;
    }
    #ifdef WIN32
        WSACleanup();
    #endif // WIN32
    return 0;
}
