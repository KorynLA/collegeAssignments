/*********************************************************************
** Sources used:
** https://www.dyclassroom.com/c/c-passing-structure-pointer-to-function
** http://www.cplusplus.com/forum/beginner/113003/
** http://www.cplusplus.com/forum/beginner/167720/
** http://www.cplusplus.com/reference/string/string/find/
** https://stackoverflow.com/questions/5958817/getaddrinfo-error-ai-socktype-not-supported
** https://kokkachiprogramming.wordpress.com/2012/11/12/how-to-convert-string-to-const-char-in-c/
** https://beej.us/guide/bgnet/html/multi/syscalls.html
** https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
*********************************************************************/

/*********************************************************************
** Program Filename: chatclient.cpp
** Author: Koryn Leslie-Arcaya
** Date: 7/28/18
** Description: creates the client side application to send/ receive messages from server.
** Input: hostname and port number using command line
** Output: connection to send/receive messages from server. Exit when '\quit' command is used. 
*********************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include<netinet/in.h>

using namespace std;


/*********************************************************************
** Description: creates the connection between server and cliet 
** Parameters: The returned integer socket descriptor when the socket was created and the client information.
** Pre & Post-Conditions: The socket was created correctly and the flags/ struct addrinfo is correct for a TCP connection.
** returns an integer to describe if the connection was successful. 
*********************************************************************/
int initiate(int sockDescrip, struct addrinfo *clientInfo);

/*********************************************************************
** Description: Takes the user prompt & handle and sends it to the server through the established connection.
** Parameters: The previously created socket and the message the user wants to send with the handle attached.
** Pre & Post-Conditions: The socket must have been correctly created and the connection must have been
** correctly initiated. The function will continue to run until the message has been sent to the server without
** error.
*********************************************************************/
void sending(int sockDescrip, string message);

/*********************************************************************
** Description: retrieves a 10 char string from the user to be used as their "handle"
** Parameters: Nothing is passed to the function
** Pre & Post-Conditions: The retrieved string from the user is less than 10 chars.
*********************************************************************/
string getUserHandle();

/*********************************************************************
** Description: Retrieves the server prompt and handle through the established connection. If the message 
** has the command '\quit' it will change the prompt to quit
** Parameters: The previously created socket descriptor and user prompt (message).
** Pre & Post-Conditions: The socket must have been correctly created and the connection must have been
** correctly initiated. The function will continue to run until the message has been retrieved from the server without
** error.
*********************************************************************/
void receive(int sockDescrip, string &prompt);

int main(int argc, char** argv){

	//retrieve user handle
	string handle = getUserHandle();	
	handle.append("> ");
	
	//Information to create the socket
	int status, sockDescrip;
	struct addrinfo client;
	struct addrinfo *clientInfo;

	memset(&client, 0, sizeof(struct addrinfo));

	//https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
	client.ai_family = AF_UNSPEC;
	client.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(argv[1], argv[2], &client, &clientInfo);
	if(status != 0) {
	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		cout << "Error creating socket.";
		return 1;
	}
	//create the socket
	//https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
	sockDescrip = socket(clientInfo->ai_family, clientInfo->ai_socktype, 0);
	if(sockDescrip == -1){
		cout << "Error with socket descriptor.";
		return 1;
	} 

	//make the connection or exit program if unable.
	int connected = initiate(sockDescrip, clientInfo);
	if(connected == 1)
		return 1;

	string prompt;
	//send and receive to/from server until client writes \quit
	do{
		prompt = "";
		cout << handle;
		getline(cin, prompt);

		if(prompt == "\\quit") {
			sending(sockDescrip, prompt);
			break;	
		}
		else {
			//add the chat client handle to what will be sent to server, then send.
			prompt = handle + prompt;
			sending(sockDescrip, prompt);

			//receive reply from server
			receive(sockDescrip, prompt);
		}
	} while(prompt != "\\quit");

	close(sockDescrip);
	cout << "Closed the socket!" << endl;
	return 0;
}

int initiate(int sockDescrip, struct addrinfo *clientInfo) {
	int connected;
	//make the connection
	connected = connect(sockDescrip, clientInfo->ai_addr, clientInfo->ai_addrlen);
	if(connected == -1) { 
		cout << "error making connection" <<endl;
		return 1;
	}
	return 0;

}

void sending(int sockDescrip, string message) {
	int sent;
	//run send function until the message has been correctly sent to the server.
	do {
		sent = send(sockDescrip, message.c_str(), message.length(), 0);
	} while(sent == -1);
}

string getUserHandle() {

	string handle;
	int handleSize = 11;

	while(handleSize> 10) {
		cout << "What is the handle: ";
		getline(cin, handle);
		handleSize = handle.length();
	}
	
	return handle;
}

void receive(int sockDescrip, string &prompt){
	int received;
	char buf[501];
	memset(buf, 0, sizeof(buf));
	//run recv function until the message from the server has been correctly retrieved
	do {
		received = recv(sockDescrip, buf, sizeof(buf), 0);
	} while(received == -1);

	//determine if the server wants to close the connection. Change user prompt to close
	//connection.
	string buffer(buf);
	//http://www.cplusplus.com/reference/string/string/find/
	if(buffer == "\\quit") {
		prompt = "\\quit";
	}
	else {
		cout << buffer <<endl;
	}

}
