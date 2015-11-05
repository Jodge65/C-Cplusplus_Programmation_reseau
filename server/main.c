#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/**
* @author Joey
*/
int main(int argc, char* argv[])
{
    /// Initialisation Variable
    char msg[255];
    char rep[255];

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    /// Initialisation adresse Local
    struct sockaddr_in adrIn;
    memset ((char*) &adrIn, '\0', sizeof(struct sockaddr_in));
    adrIn.sin_family = AF_INET;
    adrIn.sin_port = htons(atoi(argv[1])); // Serveur : Définir le port obligatoirement !
    adrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock, (struct sockaddr*) &adrIn, sizeof(struct sockaddr));


    /// Initialisation adresse Externe
    struct sockaddr_in adrOut;

    /// Connection
    if(listen(sock, 1) == 0)
    {
        // on connecte une fils d'attente de 1
        int sizeMsg;
        int sockClient;
        while(1)
        {
            // serveur non stop
            sockClient = accept(sock, (struct sockaddr*) &adrOut, (socklen_t*) &sizeMsg);

            short int boolEnd = 0;
            while(boolEnd == 0)
            {
                /// attente récéption
                boolEnd = recv(sockClient, rep, sizeMsg, 0);
                if (boolEnd != 0)
                {
                    // si quelque chose a eu lieu
                    //printf("Test\n %d \n", boolEnd);
                    if(boolEnd == -1)
                    {
                        // si c'est une erreur
                        printf("Erreur lors de la récéption\n");
                        return -1;
                    }
                    else
                    {
                        // sinon on effectue la conversion

                        int i; // T-T
                        for(i = 0; i < sizeMsg; i++)
                        {
                            if (rep[i]  >= 'a' &&  rep[i] <= 'z')
                                msg[i] =  rep[i] - ('a' - 'A');
                        }

                        send(sockClient, msg, strlen(msg), 0);
                        printf("Texte recue : '%s' convertie en : '%s' \n", rep, msg);
                    }
                }
            }
        }
        shutdown(sock, 0);
    }
    else
    {
        printf("Problème lors de la création de la file d'attente");
        return -1;
    }


    return 0;
}



