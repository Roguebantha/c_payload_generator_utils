/* As writer of this code, I hereby put this code in the public domain.
 * Feel free to use, edit, release, and republish as the user deems.
 * Credit is appreciated but is by no means required
 * Roguebantha
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
static int sock = -1;
union address {
	char addressC[8];
	void* addressP;
	long addressV;
};
void print(char *data,unsigned int numBytes) {
        int i;
        if(data)
                if(sock == -1)
                        for(i = 0; i < numBytes; i++)
                                printf("%c",data[i]);
                else
                        send(sock,data,numBytes,0);
}
void printOffsetOf(long a,int offset) {
	if(!a) {
		printf("address passed to offset from was null...?\n");
	}
	union address addr;
	addr.addressV = a + offset;
	print(addr.addressC,8);
}
void repeatPrint(char c, unsigned int num) {
	int i;
	char* redundant = malloc(num*sizeof(char));
	if(!redundant) {
		printf("ERROR allocating memory!!\n");
		return;
	}
	for(i = 0; i < num; i++)
		redundant[i] = c;
	print(redundant,num);
	free(redundant);
}
int getReply(char* reply,unsigned int reply_len) {
	memset(reply,0,2000);
	return recv(sock, reply, reply_len,0);
}
char nullbyte[8] = "\0\0\0\0\0\0\0";
int connect_to_server(short port, char* ip) {
    static struct sockaddr_in server;
    if(sock == -1) {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            printf("Could not create socket\n");
            return -1;
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
        return 0;
    }
    printf("Connection already created!! close sock and reset to -1\n");
    return 1;
}
int sendPayload(char* payload , short port,char* ip, char * reply,unsigned int reply_len)
{
    char message[1000] , server_reply[2000];
    connect_to_server(port,ip);
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
        if(!reply) {
            puts("Server reply :");
            puts(server_reply);
        }
        else {
            strncpy(reply,server_reply,reply_len);
            return 0;
        }
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
