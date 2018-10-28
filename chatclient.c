#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


void getusername(char *username){
    printf("Please enter a one-word username, up to 10 characters: ");
    scanf("%s", username);
}

int main(int argc, char *argv[]){
    // if we have too few or too many args, exit and print error
    if(argc != 3){
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }
    char username[10];
    getusername(username);
    // create address information from the arguments passed in by
    // user
    struct addrinfo * res = create_address_info(argv[1], argv[2]);
    // create a socket from address information
    int sockfd = createSocket(res);
    // connect the socket and the address information
    connect_socket(sockfd, res);
    // create a peername buffer and exhange username and peername
    // with peer
    char servername[10];
    handshake(sockfd, username, servername);
    // chat with peer
    chat(sockfd, username, servername);
    // free up the space from address information
    freeaddrinfo(res);
}
