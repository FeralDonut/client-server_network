/*
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

int socketWork(char *host_address, char *port, struct addrinfo *res)
{
    int sockfd, status, socket_fam, socket_type;
    struct addrinfo hints;

    // clear out hints of garbage data and set as IPv4 TCP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // if the status indicator is not 0, we have an error, print it
    // otherwise, return the address information
    if((status = getaddrinfo(host_address, port, &hints, &res)) != 0){
        fprintf(stderr, "getaddrinfo error: %s\nDid you enter the correct IP/Port?\n", gai_strerror(status));
        exit(1);
    }
    
    // if the socket file descriptor is -1, exit, otherwise return it
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        fprintf(stderr, "Error in creating socket\n");
        exit(1);
    }

    if ((status = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1)
    {
        fprintf(stderr, "Error in connecting socket\n");
        exit(1);
    }
    return sockfd;
}


int sendMessage(int sockfd, char *username)
{
    int status = 0;
    char server_msg[501];
    // clear out those buffers 
    memset(server_msg,0,sizeof(server_msg));

    // grab the input from the user
    printf("%s> ", username);
    fgets(server_msg, 500, stdin);
    // if the string is \quit, we close the connection
    if (strcmp(server_msg, "\\quit\n") == 0){
        return 1;
    }
    // else, we send information to the peer
    status = send(sockfd, server_msg, strlen(server_msg) ,0);
    // if there was an error, exit
    if(status == -1){
            fprintf(stderr, "Error when sending data to client\n");
            exit(1);
    }

}


void chat(int sockfd, char *username, char *servername){
    // create buffers for input and output
    char output[503];
    int message_status;
  
    memset(output,0,sizeof(output));
    // initialize error detectors
    int num_bytes_sent = 0;
    int status;
    // clear out stdin
    fgets(output, 500, stdin);
    while(1)
    {
        message_status = sendMessage(sockfd, username);

        if(message_status == 1)
        {
            break;
        }
        // grab the message from the peer
        status = recv(sockfd, output, 500, 0);
        // if there was an error receiving, exit
        if (status == -1){
            fprintf(stderr, "Error when receiving data from host\n");
            exit(1);
        }
        else if (status == 0){ // the server closed the connection
            printf("Connection closed by server\n");
            break;
        }
        else{ // the message was ok, print it
            printf("%s> %s\n", servername, output);
        }
        // clear out the input and output buffers for the next iteration
 //       memset(input,0,sizeof(input));
   //     memset(output,0,sizeof(output));
    }
    // if we break, we close the connection
    close(sockfd);
    printf("Closed Connection\n");
}

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

    // create address information from the arguments passed in by
    // user
  //  struct addrinfo * res = create_address_info(argv[1], argv[2], *res);
    // create a socket from address information
    int sockfd = socketWork(argv[1], argv[2], res);
    // connect the socket and the address information
  //  connect_socket(sockfd, *res);
    // create a peername buffer and exhange username and peername
    // with peer
    
    handshake(sockfd, username, servername);
    // chat with peer
    chat(sockfd, username, servername);
    // free up the space from address information
    freeaddrinfo(res);
}
