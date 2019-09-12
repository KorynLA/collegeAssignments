1) To start the program:
Start the server before starting the client!
In one window start the server:
gcc ftserver.c
./a.out PORTNUM
->PORTNUM must be an integer not already in use.
-> Do not run the program with the same PORTNUM within seconds of eachother. The socket 
closure takes a bit.

2) In another window start the client with the command arguments in the exact way shown:
chmod +x ftclient.py
./ftclient.py host PORTNUM command (file if applicable) PORTNUM2
->PORTNUM must be same as the on the server is running. Otherwise connection will not occur.
-> The host must be the server the server program(chat ftserve.c) is currently running on
-> The applicable commands are -l and -g. If -g is used, a file name will need to be provided as the next command argument.
-> PORTNUM2 can be any 4 digit number as long as it is not already in use as PORTNUM.