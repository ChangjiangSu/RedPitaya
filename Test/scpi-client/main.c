/**
 * $Id: $
 *
 * @brief A simple Test SCPI client. Can send one pre-defined message and read back one answer :)
 *
 * @Author Red Pitaya
 *
 * (c) Red Pitaya  http://www.redpitaya.com
 *
 * This part of code is written in C programming language.
 * Please visit http://en.wikipedia.org/wiki/C_(programming_language)
 * for more details on the language used herein.
 */


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 


      
   //strcpy(recvBuff, "SOUR:DIG:DATA:BIT LED4,1;\r\nSOUR:DIG:DATA:BIT LED5,1;\r\nSOUR:DIG:DATA:BIT LED6,1;\r\n");

    strcpy(recvBuff,
            /* LED ON */
            "DIG:PIN LED4,1;\r\nDIG:PIN LED5,1;\r\nDIG:PIN LED6,1;\r\nDIG:PIN:DIR IN,DIO1_P;\r\nANALOG:PIN AOUT0,1;\r\nANALOG:PIN? AOUT2\r\n");

    if (send(sockfd, recvBuff, strlen(recvBuff), 0) == -1) {
        perror("send");
    }


   printf("Sent message %s\n", recvBuff	);
	

    while ( (n = (int) read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
	    fputs("Received: ", stdout);
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
	    fflush(stdout);

    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}
