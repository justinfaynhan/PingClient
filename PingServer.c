/** UDP server-side ping program.
 * ------------------------------
 * Server sits in an infinite loop listening for incoming ping packets. 
 *      When a packet comes in, the server sends the packet back to the 
 *      client.
 * 
 * Author: Justin Nhan
 * Date: 1/4/2019
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define BUFFER_SIZE 1024 
  
int main (int argc, char **argv) { 
    int sockfd; 
    char buffer[BUFFER_SIZE]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    if (argc < 3) {
        printf ("usage: %s <host> <port>\n", argv[0]);
        exit (1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror ("Error : Socket creation failed."); 
        exit (EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr (argv[1]); 
    servaddr.sin_port = htons (atoi (argv[2])); 
      
    if (bind(sockfd, (const struct sockaddr *)&servaddr, 
        sizeof(servaddr)) < 0 ) { 
        perror ("Error : Binding socket failed."); 
        exit (EXIT_FAILURE); 
    } 
      
    int len, n; 
    while (true) {
        n = recvfrom(sockfd, (char *)buffer, sizeof (buffer), MSG_WAITALL, 
            (struct sockaddr *) &cliaddr, &len); 
        buffer[n] = '\0'; 
        printf("Received client : %s\n", buffer); 
        sendto(sockfd, buffer, sizeof(buffer), 0, 
            (struct sockaddr *) &cliaddr, len); 
        memset (buffer, BUFFER_SIZE, '\0');
    }
      
    return 0; 
} 
