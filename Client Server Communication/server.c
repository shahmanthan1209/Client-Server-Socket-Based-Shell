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

static int client_cnt = 0;

void myfunction(int client, int client_no)
{
  int client_points[2],Server_points[2];
  client_points[0]=0;
  client_points[1]=0;
  Server_points[0]=0;
  Server_points[1]=0;
  char message_game[255];

  while(1)
  {
    //Read operation//
    if(read(client, &Server_points, sizeof(Server_points))<0)
    {
      fprintf(stderr, "read() error\n");
      exit(3);
    }
    if(Server_points[0] == 0)
    {
      printf("\n\n Client %d has left the game. \n\n",client_no);
      close(client);
      exit(5);      
    }
    printf("\n\t\t\t#### Client %d has roll the dice ###\n",client_no);
    printf("Client score :-\n");
    printf("Client dice value : %d\n",client_points[0]);
    printf("Total client score : %d\n\n\n",client_points[1]);
    //Read operation finished//


    //Write operation//
    if(client_points[1] >= 100)
    {
      write(client,"Game over: You won the game.",50);
      printf("Client won!!!\n");
      printf("\n\n Client %d is gone. \n\n",client_no);  
      close(client);
      exit(5);
    }
    //Write operation finished//
      
      
    //Write operation//
    Server_points[0] = (rand() % 6)+1;
    Server_points[1]+=Server_points[0];
    printf("Server score :-\n");
    printf("Server dice value: %d\n", Server_points[0]);
    printf("Total server score : %d\n", Server_points[1]);
    write(client, &Server_points, sizeof(Server_points));
    //Write operation finished//
      
      
    //Write operation//      
    if(Server_points[1] >= 100)
    {
      write(client,"Game over: You lost the game.",50);      


      printf("\n\n Client %d is gone. \n\n",client_no);
        
      close(client);
      exit(5);
    }
    //Write operation finished//
      
      
    //Write operation//
    write(client,"Game on: you can now play your dice",50);
    //Write operation finished//      
    printf("\n\n*************************************************************************************\n");
  }
    exit(1); 
}

int main(int argc, char *argv[]){  

  srand(time(NULL));
  pid_t p;
  int n;
  int s_d, client, port_no;
  socklen_t len;
  char *current_Time;
  time_t currentUnixTime;

  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  s_d = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &port_no);
  servAdd.sin_port = htons((uint16_t)port_no);
  
  bind(s_d, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(s_d, 5);

  while(1){
    client = accept(s_d, NULL, NULL);
    client_cnt++;
    printf("\n\n======>> Client %d is available <<======\n\n",client_cnt);
    if(!fork())
    {
      myfunction(client, client_cnt);
    }    
    // wait(&n);
  }
    close(client);  
}
