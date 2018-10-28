#!/bin/python

"""
 Name: Jose-Antonio D. Rubio
 OSUID: 932962915
 Class: 372-400
 Program 1 chatserver

 References: https://www.bogotobogo.com/python/python_network_programming_tcp_server_client_chat_server_chat_client_select.php
             Lecture 15
             https://docs.python.org/2/howto/sockets.html
"""
from socket import *
import sys
import select

     

"""
"""
def receiveMessage(connection_socket):
    client_msg = connection_socket.recv(501)[0:-1]
    return client_msg
 
"""
 NAME
    sendMessage
 SYNPOSIS
    Takes in the connection socket,  username
 DESCRIPTION
   takes in message from client and displays with client name
   awaits a response to be entered in and then sends response to client with username
"""
def sendMessage(connection_socket, username):
        response_msg = ""
        #message to be sent back is no greater than 500 characters
        while len(response_msg) == 0 or len(response_msg) > 500:
            response_msg = raw_input("{}> ".format(username))
            
        #check to see if server side has decided to quit
        if response_msg == "\quit":
            print "Connection closed by server"
            connection_socket.close()

        #send response message back to client
        connection_socket.send(response_msg)


"""
 NAME
    handshake
 SYNPOSIS
    Takes in the connection socket and the username
 DESCRIPTION
    gets the name from the client, sends username to the client and returns
    client name to calling function
"""
def handshake(connection_socket, username):
    # get the client's name
    client_name = connection_socket.recv(1024)
    # send our username to the client
    connection_socket.send(username)
    return client_name


def chat(connection_socket, client_name, username):
     
    while 1:
        client_msg = receiveMessage(connection_socket)

       #check to see if connection was closed
        if client_msg == "":
            print "Connection closed"
            print "Waiting for new connection"
            break
        #display to terminal clients name and message formatted according to specs
        print "{}> {}".format(client_name, client_msg)

        sendMessage(connection_socket, username)

"""
"""
def startUp():            
    #set port to first argument passed in, set up the socket and listen
    #reference: lecture 15
    serverport = sys.argv[1]
    serversocket = socket(AF_INET, SOCK_STREAM)
    serversocket.bind(('', int(serverport)))
    serversocket.listen(1)

    username = ""
    while len(username) == 0 or len(username) > 10:
        username = raw_input("Please enter a one-word username, up to 10 characters: ")
        

    print "The server is ready to receive"


    while 1:
        connection_socket, addr = serversocket.accept()
        client_name = handshake(connection_socket, username)
        print "Ready to chat with {}".format(client_name)
       
        # chat with the incoming connection, handshake with them first
        chat(connection_socket, client_name, username)
    
   
   

"""
Main
"""
if __name__ == "__main__":
   
    if len(sys.argv) != 2:
        print "Incorrect number of arguments"
        exit(1)

    startUp() 
exit(0)
 