/*
 Name: Jose-Antonio D. Rubio
 OSUID: 932962915
 Class: 372-400
 Program 1 chatclient
 References: https://www.bogotobogo.com/python/python_network_programming_tcp_server_client_chat_server_chat_client_select.php
             Lecture 15
             https://docs.python.org/2/howto/sockets.html
             https://linux.die.net/man/2/send
             http://beej.us/guide/bgnet/html/single/bgnet.html#closedown
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
 NAME
    getUsername
 SYNOPSIS
    takes in a pointer to a char array to store username
 DESCRIPTION
    Prompts user for a username and stores it in char array
*/
void getUsername(char *username)
{
    printf("Please enter a one-word username, up to 10 characters: ");
    scanf("%s", username);
}

/*
 NAME
    socketWork
 SYNOPSIS
    takes in a pointer the command ling inouts for the host address and port number as well as the addrinf struct
 DESCRIPTION
    Creates and connects the socket with the server
*/
int socketWork(char *host_address, char *port, struct addrinfo *res)
{
    int sockfd, status, socket_fam, socket_type;
    struct addrinfo hints;

    // clear out hints of garbage data and set as IPv4 TCP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    
    if((status = getaddrinfo(host_address, port, &hints, &res)) != 0){
        fprintf(stderr, "getaddrinfo error: %s\nDid you enter the correct IP/Port?\n", gai_strerror(status));
        exit(1);
    }
    
    // create socket and check if there was an error
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        fprintf(stderr, "Error in creating socket\n");
        exit(1);
    }

    //connect socket and check if there was an error
    if ((status = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1)
    {
        fprintf(stderr, "Error in connecting socket\n");
        exit(1);
    }
    return sockfd;
}

/*
 NAME
    sendMessage
 SYNOPSIS
    takes in sockfd and pointer to user name and returns an int to determine if client quit
 DESCRIPTION
    prompts user for a message to deliver,
    checks to see if it is a quit
    if not then sends it through the socket
*/
int sendMessage(int sockfd, char *username)
{
    int status = 0;
    char client_msg[501];
    // clear out those buffers 
    memset(client_msg,0,sizeof(client_msg));

    // grab the input from the user
    printf("%s> ", username);
    fgets(client_msg, 500, stdin);
    // if the string is \quit, we close the connection
    if (strcmp(client_msg, "\\quit\n") == 0){
        return 1;
    }
    // else, we send information to the peer
    status = send(sockfd, client_msg, strlen(client_msg) ,0);
    // if there was an error, exit
    if(status == -1){
            fprintf(stderr, "Error when sending data to client\n");
            exit(1);
    }
return 0;
}

/*
 NAME
    receiveMessage
 SYNOPSIS
    takes in sockfd and pointer to server name
 DESCRIPTION
    receives server message
    checks to see if it is a quit
    if not displays to stdout
*/
void receiveMessage(int sockfd, char *servername)
{
    char server_msg[501];
    int status;
    //clear out buffer
    memset(server_msg,0,sizeof(server_msg));
   //start a new line
    
    // grab the message from the peer
    status = recv(sockfd, server_msg, 500, 0);
    // if there was an error receiving, exit
    if (status == -1)
    {
        fprintf(stderr, "Error when receiving data from host\n");
        close(sockfd);
        exit(1);
    }else if (status == 0)
    { 
        printf("Connection closed by server\n");
        close(sockfd);
        exit(1);
    }else
    { // the message was ok, print it
        printf("%s> %s\n", servername, server_msg);
    }

}

/*
 NAME
    chat
 SYNOPSIS
    takes in sockfd and pointer to username and server name
 DESCRIPTION
    initiates chat and runs continuour while loop of
    sendMessage and receiveMessage
*/
void chat(int sockfd, char *username, char *servername){
    // create buffers for input and output
    char clear_stdin[500];
    int message_status;
  
    memset(clear_stdin,0,sizeof(clear_stdin));

    // clear out stdin
    fgets(clear_stdin, 500, stdin);
    while(1)
    {
        message_status = sendMessage(sockfd, username);

        if(message_status == 1)
        {
            break;
        }
        
        receiveMessage(sockfd, servername);
    }
    // if we break, we close the connection
    close(sockfd);

}

/*
 NAME
    handshake
 SYNOPSIS
    takes in sockfd and pointer to username and server name
 DESCRIPTION
    sends server username and receives the servername and checks for errors
*/
void handshake(int sockfd, char *username, char *server_name){
    // send our username to the peer
    int sending_name, client_name;
    if ((sending_name = send(sockfd, username, strlen(username), 0)) == -1)
    {
        fprintf(stderr, "Error in sending username to client\n");
        exit(1);
    }
    // grab the username of our peer and place into servername
    if ((client_name = client_name = recv(sockfd, server_name, 10, 0)) == -1)
    {
        fprintf(stderr, "Error in receiving username to client\n");
        exit(1);
    }
}

/*
MAIN
*/

int main(int argc, char *argv[])
{
    char username[10];
    char servername[10];
    struct addrinfo *res;

    if(argc != 3)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }
    
    getUsername(username);

    int sockfd = socketWork(argv[1], argv[2], res);

    handshake(sockfd, username, servername);
 
    chat(sockfd, username, servername);

    freeaddrinfo(res);
}