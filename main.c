//
// Created by finnj on 19.04.2022.
//

#include"main.h"
#include<stdio.h>
#include "stdlib.h"
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#define PORT 5678

int main(int argc , char *argv[]) {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    socklen_t addr_size;
    char client_message[2000];
    pid_t childpid;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
        exit(1);
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    memset(&server, '\0', sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    if (listen(socket_desc, 10) == 0) {
        printf("Waiting for incoming connections...\n");
    } else {
        printf("Error in binding.\n");
    }

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    while (1) {
        client_sock = accept(socket_desc, (struct sockaddr *) &client, &addr_size);
        if (client_sock < 0) {
            perror("accept failed");
            exit(1);
        }
        puts("Connection accepted");

        if((childpid = fork()) == 0) {
            close(socket_desc);

            while(1) {
                recv(client_sock, client_message, 2000, 0);
                if (strcmp(client_message, ":exit") == 0) {
                    printf("Disconnected\n");
                    break;
                } else {
                    printf("Client: %s\n", client_message);
                    send(client_sock, client_message, strlen(client_message), 0);
                    bzero(client_message, sizeof(client_message));
                }
            }
        }

    }
    close(client_sock);
    return 0;
}
