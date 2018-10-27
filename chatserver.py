#!/bin/python

"""
 Name: Jose-Antonio D. Rubio
 OSUID: 932962915
 Class: 372-400
 Program 1 chatserver

 References: https://www.bogotobogo.com/python/python_network_programming_tcp_server_client_chat_server_chat_client_select.php
             Lecture 15
"""
from socket import *
import sys
import select


"""
 NAME
    chat
 SYNPOSIS
    Takes in the connection socket, client name and the username
 DESCRIPTION
   takes in message from client and displays with client name
   awaits a response to be entered in and then sends response to client with username
"""
    """Chat
    Initiates a chart session with the client
    Lets them send the first messages
    """

def chat(connection_socket, clientname, username):

    response_msg = ""
   
    while 1:
    client_msg = connection_socket.recv(501)[0:-1]
   
   #check to see if connection was closed
    if client_msg == "":
        print "Connection closed"
        print "Waiting for new connection"
        break
    #display to terminal clients name and message formatted according to specs
    print "{}> {}".format(clientname, client_msg)

    
    response_msg = ""
    #message to be sent back is no greater than 500 characters
    while len(response_msg) == 0 or len(response_msg) > 500:
        response_msg = raw_input("{}> ".format(username))
        
    #check to see if server side has decided to quit
    if response_msg == "\quit":
        print "Connection closed"
        print "Waiting for new connection"
        break

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
    clientname = connection_socket.recv(1024)
    # send our username to the client
    connection_socket.send(username)
    return clientname

"""
"""
  def chatServer():
   #set port to first argument passed in, set up the socket and listen
   #reference: lecture 15
   serverport = sys.argv[1]
   serversocket = socket(AF_INET, SOCK_STREAM)
   serversocket.bind(('', int(serverport)))
   serversocket.listen(1)

    username = ""
    while len(username) == 0 or len(username) > 10:
        username = raw_input("Usernames must be a one-word name, up to 10 characters")
        

    print "The server is ready to receive"
   

    while 1:
        connection_socket, addr = serversocket.accept()
        print "Chat server started on port  {}".format(addr)
       
        # chat with the incoming connection, handshake with them first
        chat(connection_socket, handshake(connection_socket, username), username)
        connection_socket.close()

"""
Main
"""
if __name__ == "__main__":
   
    if len(sys.argv) != 2:
        print "Incorrect number of arguments"
        exit(1)

    sys.exit(chat_server()) 
 