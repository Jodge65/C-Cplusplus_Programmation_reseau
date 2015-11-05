#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>

#if defined (linux) || defined (_POSIX_VERSION) || defined (_POSIX2_C_VERSION) || defined (_XOPEN_VERSION)
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
* @name Morpion Client
*/
int main(int argc, char* argv[])
{
    #ifdef WIN32
        WSADATA WSAData;
        WSAStartup(MAKEWORD(2,0), &WSAData);
    #endif // WIN32

    int const SIZE_MSG = 10;
    char answer[SIZE_MSG];
    char say[SIZE_MSG];

    const char* ipTextuel = argv[1];

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    /// Initialisation adresse Local
    SOCKADDR_IN adrIn;
    //memset ((char*) &adrIn, '\0', sizeof(SOCKADDR_IN));
    adrIn.sin_family = AF_INET;
    adrIn.sin_port = htons(0);
    adrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock, (SOCKADDR*) &adrIn, sizeof(adrIn));


    /// Initialisation adresse Externe
    SOCKADDR_IN adrOut;
    //memset ((char*) &adrOut, '\0', sizeof(SOCKADDR_IN));
    adrOut.sin_family = AF_INET;
    adrOut.sin_port = htons(atoi(argv[2])); // htons(10500);
    adrOut.sin_addr.s_addr = inet_addr(ipTextuel);

    /// Connection
    if(connect(sock, (SOCKADDR*) &adrOut, sizeof(adrOut)) == 0)
    {
        bool partieTerminer = false;
        bool validate = false;
        while(!partieTerminer)
        {
            if(!validate)
                printf("En attente de l'autre joueur...\n");
            recv(sock, answer, SIZE_MSG, 0);
            if(strcmp(answer, "quit")  == 0)
            {
                partieTerminer = true;
            }
            else if(strcmp(answer, "ewv")  == 0 || strcmp(answer, "ERREUR"))
            {
                // Error Wrong Value
                printf("[ERREUR] Valeurs incorrecte !\nValeur autorisé : 1..9 ou QUIT%s:", validate? " ": " ou ACP ");
            }
            else if(strcmp(answer, "eap")  == 0)
            {
                // Error Already Play
                printf("[ERREUR] Cette case a déjà été jouée :");
                validate = false;
            }
            else
            {
                for(int i = 0; i < SIZE_MSG; i++)
                {
                    if(!(answer[i] =='O' || answer[i] == 'X'))
                    {
                        answer[i] = ' ';
                    }
                }

                printf("TOUR ? #----------------------------------------------\n");
                printf("Grille :\n");
                printf("╔═╦═╦═╗\n");
                printf("║%c║%c║%c║\n", answer[6], answer[7], answer[8]);
                printf("╠═╬═╬═╣\n");
                printf("║%c║%c║%c║\n", answer[3], answer[4], answer[5]);
                printf("╠═╬═╬═╣\n");
                printf("║%c║%c║%c║\n", answer[0], answer[1], answer[2]);
                printf("╚═╩═╩═╝\n");

                if(!validate)
                {
                    printf("Jouer : ");
                }
                else
                {
                    printf("Validé (ACP) : ");
                }
                validate = !validate;
            }

            if(!partieTerminer)
            {
                scanf("%s", &say);
                send(sock, say, SIZE_MSG, 0);
            }
        }

    }
    else
    {
        printf("Connection non-établie\n");
        return -1;
    }

    shutdown(sock, 0);
    #ifdef WIN32
        WSACleanup();
    #endif // WIN32
    return 0;
}
