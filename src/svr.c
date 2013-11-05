#include "head/unp.h"
#include "head/svr.h"
#include "head/log.h"

#define CONNMAX 10
#define BYTES 1024

int listenfd, clients[CONNMAX];

void MainServer(char *dir, int port)
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    printf("Server started at port no. %s%d%s with root directory as %s%s%s\n","\033[92m",port,"\033[0m","\033[92m",dir,"\033[0m");
    int slot=0;
    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i = 0; i<CONNMAX; i++)
        clients[i]=-1;
    startServer(port);

    // ACCEPT connections
    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[slot]<0)
            ErrorLog("WARNING", "accept() error");
        else
        {
            if ( fork()==0 )
            {
                respond(slot, dir);
                exit(EXIT_SUCCESS);
            }
        }

        printf("\n\nbefore: clients[slot]: %d", clients[slot]);

        while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;

        printf("\n\nafter: clients[slot]: %d", clients[slot]);
    }
}

//start server
void startServer(int port)
{
    struct addrinfo hints, *res, *p;
    char port_str[5];
	sprintf(port_str, "%d", port);

    // getaddrinfo for host
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port_str, &hints, &res) != 0)
    {
        ErrorLog("FATAL", "getaddrinfo() error");
        exit(EXIT_FAILURE);
    }
    // socket and bind
    for (p = res; p!=NULL; p=p->ai_next)
    {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL)
    {
        ErrorLog("FATAL", "socket() or bind() error");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        ErrorLog("FATAL", "listen() error");
        exit(EXIT_FAILURE);
    }
}

//client connection
void respond(int n, char *dir)
{
    char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read;

    memset( (void*)mesg, (int)'\0', 99999 );

    rcvd=recv(clients[n], mesg, 99999, 0);

    if (rcvd<0)    // receive error
        ErrorLog("FATAL", "recv() error");
    else if (rcvd==0)    // receive socket closed
        AccessLog("124.33.3.3", "Client disconnected upexpectedly.");
    else    // message received
    {
        printf("%s", mesg);
        reqline[0] = strtok (mesg, " \t\n");
        if ( strncmp(reqline[0], "GET\0", 4)==0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
            }
            else
            {
                if ( strncmp(reqline[1], "/\0", 2)==0 )
                    reqline[1] = "/index.html";        //Because if no file is specified, index.html will be opened by default (like it happens in APACHE...

                strcpy(path, dir);
                strcpy(&path[strlen(dir)], reqline[1]);
                printf("file: %s\n", path);

                if ( (fd=open(path, O_RDONLY))!=-1 )    //FILE FOUND
                {
                    send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
                        write (clients[n], data_to_send, bytes_read);
                }
                else    write(clients[n], "HTTP/1.0 404 Not Found\n", 23); //FILE NOT FOUND
            }
        }
    }

    //Closing SOCKET
    shutdown (clients[n], SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clients[n]);
    clients[n]=-1;
}



