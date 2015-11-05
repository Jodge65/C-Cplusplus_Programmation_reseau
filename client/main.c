#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>



int main(int argc, char* argv[])
{


    /// Initialisation Variable
    char msg[255];
    char rep[255];

    const char* ipTextuel = argv[1]; // "10.3.203.15";

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    /// Initialisation adresse Local
    struct sockaddr_in adrIn;
    memset ((char*) &adrIn, '\0', sizeof(struct sockaddr_in));
    adrIn.sin_family = AF_INET;
    adrIn.sin_port = htons(0);
    adrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock, (struct sockaddr*) &adrIn, sizeof(struct sockaddr));


    /// Initialisation adresse Externe
    struct sockaddr_in adrOut;
    memset ((char*) &adrOut, '\0', sizeof(struct sockaddr_in));
    adrOut.sin_family = AF_INET;
    adrOut.sin_port = htons(atoi(argv[2])); // htons(10500);
    adrOut.sin_addr.s_addr = inet_addr(ipTextuel);


    /// Connection
    if(connect(sock, (struct sockaddr*) &adrOut, sizeof(struct sockaddr)) == 0)
    { // Si la connection est établie
        /// Demande msg
        printf("Veuillez entrer une chaine de caractère : ");
        scanf("%s", msg);

        short int boolEnd = 0;
        while(boolEnd == 0)
        {
            /// envoie message
            send(sock, msg, strlen(msg), 0);

            /// attente récéption
            boolEnd = recv(sock, rep, strlen(msg), 0);
        }
        shutdown(sock, 0);

        if(boolEnd == -1)
        {
            printf("Erreur lors de la récéption");
            return -1;
        }
        else
        {
                printf("Votre texte '%s' est désormais : '%s' \n", msg, rep);
        }

    }
    else
    {
                printf("Connection non-établie");
                return -1;
    }



    return 0;
}
