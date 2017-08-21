#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
union address {
	char addressC[8];
	void* addressP;
	long addressV;
};
print(char *data,unsigned int numBytes) {
	int i;
	if(data)
		for(i = 0; i < numBytes; i++)
			printf("%c",data[i]);
}
printOffsetOf(long a,int offset) {
	if(!a) {
		printf("address passed to offset from was null...?\n");
	}
	union address addr;
	addr.addressV = a + offset;
	print(addr.addressC,8);
}
repeatPrint(char c, unsigned int num) {
	int i;
	for(i = 0; i < num; i++)
		printf("%c",c);
}
char nullbyte[8] = "\0\0\0\0\0\0\0";
int sendPayload(char* payload , short port,char* ip)
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket\n");
    }
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("Connected successfully to server.\n");
    if(payload) {
        printf("Sending payload...\n");
        if(send(sock, payload, strlen(payload), 0) < 0) {
            printf("Failed to send payload.\n");
            return 1;
        }
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            return 1;
        }
        puts("Server reply :");
        puts(server_reply);
    }
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        puts("Server reply :");
        puts(server_reply);
    }
    close(sock);
    return 0;
}
