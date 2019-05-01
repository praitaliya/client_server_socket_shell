#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <time.h>

static int clientCnt = 0;

void myFunct(int client, int clientNo)
{
  int clientPts[2],ServerPts[2];
  clientPts[0]=0;
  clientPts[1]=0;
  ServerPts[0]=0;
  ServerPts[1]=0;
  char gameMsg[255];

  while(1)
  {
    //Read operation//
    if(read(client, &ServerPts, sizeof(ServerPts))<0)
    {
      fprintf(stderr, "Error in method : read() \n");
      exit(3);
    }
    if(ServerPts[0] == 0)
    {
	  printf("\n\n Client %d left the game. \n\n",clientNo);
      close(client);
      exit(5);      
    }

	printf("\n Client %d plays \n",clientNo);
    printf("Client score : \n");
    printf("Client dice value : %d\n",clientPts[0]);
    printf("Total client score : %d\n\n\n",clientPts[1]);

    //Read operation finished//


    //Write operation//
    if(clientPts[1] >= 100)
    {

	  write(client,"Game Over: You won ...!! ",50);
      printf("Client won...!!\n");
      printf("\n\n Client %d is done. \n\n",clientNo);  
      close(client);
      exit(5);
    }
    //Write operation finished//
      
      
    //Write operation//
    ServerPts[0] = (rand() % 6)+1;
    ServerPts[1]+=ServerPts[0];
	
	printf("Server score : \n");
    printf("Server current dice value : %d\n", ServerPts[0]);
    printf("Total server score : %d\n", ServerPts[1]);
    write(client, &ServerPts, sizeof(ServerPts));
    //Write operation finished//
      
      
    //Write operation//      
    if(ServerPts[1] >= 100)
    {
      write(client,"Game over: You lost ...!!",50);      

      printf("\n\n Client %d is done. \n\n",clientNo);
        
      close(client);
      exit(5);
    }
    //Write operation finished//
      
      
    //Write operation//
    write(client,"Game is on: you can now play",50);
    //Write operation finished//      
    printf("\n <<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>> \n");
  }
    exit(1); 
}

int main(int argc, char *argv[]){  

  srand(time(NULL));
  int s_d, client, port_no;
  socklen_t len;
  char *currTime;
  time_t currentUnixTime;

  struct sockaddr_in servAddr;     // server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  s_d = socket(AF_INET, SOCK_STREAM, 0);
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &port_no);
  servAddr.sin_port = htons((uint16_t)port_no);
  
  bind(s_d, (struct sockaddr *) &servAddr, sizeof(servAddr));
  listen(s_d, 5);

  while(1){
    client = accept(s_d, NULL, NULL);
    clientCnt++;
    printf("\n\n <<<<<<<<<< Client %d is available >>>>>>>>>> \n\n",clientCnt);
    if(!fork())
    {
      myFunct(client, clientCnt);
    }    
  }
    close(client);  
}
