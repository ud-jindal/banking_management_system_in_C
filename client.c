#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

void error(char* msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[]) {
  int sockfd, port_no, n;
  struct sockaddr_in server_add;
  struct hostent *server;

  char buffer[256];

  port_no = 8887;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  //server = gethostbyname(argv[1]);

  bzero(&server_add, sizeof(server_add));

  server_add.sin_family = AF_INET;
  server_add.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_add.sin_port = htons(port_no);

  if(connect(sockfd, (struct sockaddr *) &server_add, sizeof(server_add)) < 0) {
    error("ERROR on connect");
  }

  printf("Welcome to online banking management system\nPlease enter your login type\n0: admin\n1: normal\n2: joint\n");
  int login, result;
  scanf("%d", &login);
  write(sockfd, &login, sizeof(login));
  char username[10], pin[4];
  printf("Enter your login details\nUsername: \n");
  scanf("%s\n",username);
  write(sockfd, username, 10);
  printf("Pin: \n");
  scanf("%s\n",pin);
  write(sockfd, pin, 4);
  read(new_sock_fd, &result, sizeof(result));
  // 0: Admin, 1: normal, 2: joint
  if(result) {
    if(login == 0) {
      int operation;
      printf("Select operation you want to do\n0: add, 1: delete, 2: modify, 3: search");
      scanf("%d", &operation);
      write(sockfd, &operation, sizeof(operation));
      if(operation == 0) {
        int type;
        printf("What type of user you want to add\n0: normal, 1: joint");
        scanf("%d", &type);
        write(sockfd, &type, sizeof(type));
      }
    }
    else if(login == 1 || login == 2) {
      int operation;
      scanf("%d", &operation);
      write(sockfd, &operation, sizeof(operation));
    }
    else {

    }
  }
  else {

  }


}
