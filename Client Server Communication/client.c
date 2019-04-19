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
  int server, port_no;
  socklen_t len;
  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &port_no);
  servAdd.sin_port = htons((uint16_t)port_no);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
  }

 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }


  int Client_points[2], server_points[2];
  Client_points[0]=0;
  Client_points[1]=0;
  server_points[0]=0;
  server_points[1]=0;
  char c;

  while(1)
  {

    printf("\n------------------ To Play press Enter || To Quit press 'q' -------------------\n");
    
    while(1)
    {
        
     //write operation//
      c=fgetc(stdin);
      
      if(c==0x0A)
      {
        Client_points[0] = (rand() % 6)+1;
        Client_points[1]+=Client_points[0];
        break;
      }
      else if(c=='q')
      {
        Client_points[0]=0;
        Client_points[1]=0;
        write(server, &Client_points, sizeof(Client_points));
        printf("\n\n I am quiting this game, Good bye server. \n");
        exit(7);        
      }
      else
      {
        Client_points[0]=0;
        printf("To Play press Enter");
      }
    }    

    printf("\nClient score :-\n");
    printf("Dice : %d\n", Client_points[0]);
    printf("Total score : %d\n", Client_points[1]);
    printf("\n\n\n");
    write(server, &Client_points, sizeof(Client_points));
    //write operatin finished//
      
      
    //Read operation//
    if(Client_points[1] >= 100)
    {
      if (read(server, &msg, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      
      printf("\n\nMessage From server :: %s \n",msg);
      close(server);
      exit(5);
    }
    //Read opration finished//
      
      
    //Read operation//  
    if (read(server, &server_points, sizeof(server_points))<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }
    printf("\nServer score :-\n");    
    printf("Dice :%d \n",server_points[0]);
    printf("Total server score : %d\n",server_points[1]);
    //Read opration finished//
      
      
    //Read operation//  
    if(server_points[1] >= 100)
    {
      if (read(server, &msg, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      printf(" Message From server :: %s \n",msg);
      close(server);
      exit(5);
    }
    //Read opration finished//


    //Read operation//  
    if (read(server, &msg, 50)<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }

    printf("\n\n Message From server :: %s \n",msg);
    //Read opration finished//
  }

  exit(0);
}

