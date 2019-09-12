#!/usr/bin/env python
#!/usr/bin/python

#####################################################################
#Sources used:
#https://docs.python.org/release/2.6.5/library/socket.html#module-socket
#https://stackoverflow.com/questions/34249188/oserror-errno-107-transport-endpoint-is-not-connected
#https://www.w3schools.com/python/python_strings.asp
#https://stackoverflow.com/questions/31565609/python-while-loop-condition-check-for-string
#https://stackoverflow.com/questions/41014252/socket-error-errno-32-broken-pipe/41015359
#https://www.youtube.com/watch?v=DIPZoZheMTo
#https://stackoverflow.com/questions/10751948/command-line-arguments-in-python/10751992
#https://swcarpentry.github.io/python-novice-inflammation/10-cmdline/
#https://stackoverflow.com/questions/30037465/how-can-i-execute-a-python-script-by-just-typing-the-scripts-name
#####################################################################

#####################################################################
# Program Filename: chatserve.py
## Author: Koryn Leslie - Arcaya
## Date: 7/28/2018
## Description: Runs the server side of a TCP.
## Input: via command line it is given the port number the server should bind to.
## Output: A server that can send/receive messages from connections, accept/close connections, 
## and run "forever" (until supervisor hits CRTL C.
#####################################################################



import sys
import os
import socket
import string

#####################################################################
## Description: Listens and accepts connection from a client. 
## Parameters: The created socket bound to the hostname/portnumber 
## Pre & Post-Conditions: The socket must have been correctly created.
## The end result will be an accepted connection between server and client.
#####################################################################
#https://docs.python.org/release/2.6.5/library/socket.html#module-socket
def startup(socketRob):
    print("Waiting for new connection")
    socketRob.listen(1)
    conn, addr = socketRob.accept()
    return conn

#####################################################################
## Description: Sends user input and the handle to the connected client
## Parameters: The handle for server and the conected socket 
## Pre & Post-Conditions: The server must be correctly connected to a client.
## The result is the message is sent with the handle to the client. 
#####################################################################
#https://docs.python.org/release/2.6.5/library/socket.html#module-socket
def send(handle, conn):
    message = raw_input(handle)
    if message == '\quit':
        conn.send(message)
        close(conn)
    else:
        messageToSend = handle + message
        sent = conn.send(messageToSend)
    return message

#####################################################################
## Description: Receives message from connected client and returns it.  
## Parameters: The socket connection to the client
## Pre & Post-Conditions: The socket connection to the client must have been 
## correctly created. The return result is an uncorrupted message from the client.
#####################################################################
#https://docs.python.org/release/2.6.5/library/socket.html#module-socket
def receive(conn):
    return conn.recv(500)

#####################################################################
## Description: Closes socket connection between client and server.  
## Parameters: The socket connection to the client
## Pre & Post-Conditions: The socket connection to the client must have been 
## correctly created. The connection will be correctly closed so the server can 
## accept new client connections.
#####################################################################
#https://docs.python.org/release/2.6.5/library/socket.html#module-socket
def close(conn):
    conn.close()
    print("closed connection")

#####################################################################
## Description: main runs the server side. Accepts/ends connections from clients and sends messages
## to/ from. 
## Parameters: Command line argument is the portnumber that will be used by server
## Pre & Post-Conditions: The port number should not be in use. If it is the server will
## not be able to bind to socket/ run.  
#####################################################################
def main():
    handle = "Jan> "
    #socket will be able to be reachable by any machine over server
    HOST = "flip.engr.oregonstate.edu" 
    portNum = int(sys.argv[1])

    #to create the socket - Sock_STREAM to create TCP connection
    #https://docs.python.org/release/2.6.5/library/socket.html#module-socket
    socketRob = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socketRob.bind((HOST, portNum))

    #run until CRTL C
    while 1:
        conn = startup(socketRob)
        #need to reference message before it exists
        message = ""
        while message != '\quit':
            if '\quit' != message:
                received = receive(conn)
                if '\quit' == received:
                    close(conn)
                    break;
                else:
	            print(received)
                    message = send(handle, conn)

main()
