#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


void error(char const* msg)
{
    printf(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sfd;
    int portno = 12014;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0)
        error("ERROR: Could not open socket.");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if(bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR: Binding failed.");
    listen(sfd,5);
    while(1)
    {
        struct sockaddr_in cl_addr;
        unsigned int cllen = sizeof(cl_addr);
        int nsfd = accept(sfd, (struct sockaddr*) &cl_addr, &cllen);
        if (nsfd < 0)
            error("ERROR: Accept failed.");
        char buffer[256];
        memset(&buffer, 0, 256*sizeof(char));
        int n = read(nsfd, buffer, 255);
        if(n < 0) error("ERROR: Reading failed.");
        printf("Message: %s\n", buffer);
        n = write(nsfd,"OK", 3);
        if(n < 0) error("ERROR: Writing to socket.");
        close(nsfd);
    }
    close(sfd);
    return 0;
}
