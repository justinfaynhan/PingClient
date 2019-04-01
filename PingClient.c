/** UDP client-side ping program.
 * ------------------------------
 * Sets up a UDP socket and sends NUM_PINGS datagram packets to a receiving 
 *      ping server. Time out if no server response received in TIME_OUT 
 *      seconds.
 * 
 * Author: Justin Nhan
 * Date: 1/4/2019
 */

#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <unistd.h>
  
/* Configure as appropriate */
#define NUM_PINGS 10
#define TIME_OUT 1
#define BUFFFER_SIZE 100
  
int main (int argc, char** argv) {    
    char buffer[BUFFER_SIZE]; 
    int sockfd, n; 
    struct sockaddr_in servaddr; 
    
    const char *message = "Ping Pong."; 
    
    if (argc < 3) {
        printf ("usage: %s <host> <port>\n", argv[0]);
        exit (1);
    }

    bzero (&servaddr, sizeof (servaddr)); 
    servaddr.sin_addr.s_addr = inet_addr (argv[1]); 
    servaddr.sin_port = htons (atoi (argv[2])); 
    servaddr.sin_family = AF_INET; 
      
    sockfd = socket (AF_INET, SOCK_DGRAM, 0); 
    struct timeval tv = {TIME_OUT, 0};
    setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv));
        
    if (connect (sockfd, (struct sockaddr *)&servaddr, sizeof (servaddr)) < 0) { 
        fprintf (stderr, "Error : Connect Failed \n"); 
        exit (0); 
    } 

    double maxRTT = 0;
    double minRTT = TIME_OUT;
    double sumRTT = 0;
    
    /* Send NUM_PINGS datagram pings to server.*/
    for (int i = 1; i <= NUM_PINGS; i++) {
        sendto (sockfd, message, strlen (message), 0, 
            (struct sockaddr*)NULL, sizeof (servaddr)); 
        clock_t begin = clock();

        // Waiting for response.
        int recvlen = recvfrom (sockfd, buffer, sizeof (buffer), 0, 
            (struct sockaddr*)NULL, NULL); 
        printf("ping to %s, seq = %d, ", argv[1], i);
        if (recvlen >= 0) {
            clock_t end = clock();   

            // Calculate RTT info.         
            double time_elapsed = (double)(end - begin) / 
                (CLOCKS_PER_SEC / 1000);
            if (time_elapsed > maxRTT) 
                maxRTT = time_elapsed;
            if (time_elapsed < minRTT)
                minRTT = time_elapsed;
            sumRTT += time_elapsed;
            printf ("rtt = %.3lfms\n\r", time_elapsed); 
        } else {
            printf ("time out\n\r");
        }
    }

    close (sockfd); 
    printf ("RTT summary: max = %.3lfms, min = %.3lfms, average = %lfms\n",
        maxRTT, minRTT, sumRTT/NUM_PINGS);
} 
