gcc socket_server.c -o server 2> errors1.txt
cat errors1.txt | grep error
gcc socket_client.c -o client  2> errors2.txt
cat errors2.txt | grep error

