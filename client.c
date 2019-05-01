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
#include <time.h>
#include <signal.h>


int main(int argc, char *argv[])
{
  srand(time(NULL));
  signal(SIGINT, SIG_IGN);
  char msg[100];
  int server, portNo;
  struct sockaddr_in servAddr;     // server socket address
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Error inside Socket Creation \n");
     exit(1);
  }

  servAddr.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNo);
  servAddr.sin_port = htons((uint16_t)portNo);

  if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr) < 0){
  fprintf(stderr, " Error inside inet_pton() \n");
  exit(2);
  }

 if(connect(server, (struct sockaddr *) &servAddr, sizeof(servAddr))<0){
  fprintf(stderr, " Error inside connect()\n");
  exit(3);
 }


  int clientPts[2], ServerPts[2];
  clientPts[0]=0;
  clientPts[1]=0;
  ServerPts[0]=0;
  ServerPts[1]=0;
  char ch;

  while(1)
  {

    printf("\n <<<<<<<<<< Hit Enter to Start OR Hit 'Q' to Quit >>>>>>>>>> \n");
    
    while(1)
    {
        
     //write operation//
      ch=fgetc(stdin);
      
      if(ch==0x0A)
      {
        clientPts[0] = (rand() % 6)+1;
        clientPts[1]+=clientPts[0];
        break;
      }
      else if(ch=='q')
      {
        clientPts[0]=0;
        clientPts[1]=0;
        write(server, &clientPts, sizeof(clientPts));
        printf("\n\n I Quit, Have a great Day server. \n");
        exit(7);        
      }
      else
      {
        clientPts[0]=0;
        printf("Hit Enter to Start");
      }
    }    

    printf("\nClient score :\n");
    printf("Client current dice value : %d\n", clientPts[0]);
    printf("Total client score : %d\n", clientPts[1]);
    write(server, &clientPts, sizeof(clientPts));
    //write operatin finished//
      
      
    //Read operation//
    if(clientPts[1] >= 100)
    {
      if (read(server, &msg, 50)<0){
        fprintf(stderr, "Error in method : read() \n");
        exit(3); 
      }

      printf("\n\n <<<<<<<<<< Server Message >>>>>>>>>> %s \n",msg);
      close(server);
      exit(5);
    }
    //Read opration finished//
      
      
    //Read operation//  
    if (read(server, &ServerPts, sizeof(ServerPts))<0){
      fprintf(stderr, "Error in method : read() \n");
      exit(3); 
    }
    printf("\nServer score :-\n");    
    printf("Server current dice value : %d \n",ServerPts[0]);
    printf("Total server score : %d\n",ServerPts[1]);
    //Read opration finished//
      
      
    //Read operation//  
    if(ServerPts[1] >= 100)
    {
      if (read(server, &msg, 50)<0){
        fprintf(stderr, "Error in method : read() \n");
        exit(3); 
      }
      printf(" <<<<<<<<<< Server Message >>>>>>>>>> %s \n",msg);
      close(server);
      exit(5);
    }
    //Read opration finished//


    //Read operation//  
    if (read(server, &msg, 50)<0){
      fprintf(stderr, " Error in method : read() \n");
      exit(3); 
    }

    printf("\n\n <<<<<<<<<< Server Message >>>>>>>>>> %s \n",msg);
    //Read opration finished//
  }

  exit(0);
}
