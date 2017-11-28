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

  printf("Welcome to online banking management system\n Please enter your login type\n0: admin\n1: normal\n2: joint\n");
  int login;
  scanf("%d", &login);
  write(sockfd, &login, sizeof(login));
  // 0: Admin, 1: normal, 2: joint
  if(login == 0) {
    int operation;
    scanf("%d", &operation);
    write(sockfd, &operation, sizeof(operation));
  }
  else if(login == 1 || login == 2) {
    int operation;
    scanf("%d", &operation);
    write(sockfd, &operation, sizeof(operation));
  }
  else {

  }


}
