#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(char* msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[]) {
  int sockfd, port_no, n;
  struct sockaddr_in server_add;
  struct hostent *server;

  char buffer[256];

  port_no = atoi(argv[1]);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  //server = gethostbyname(argv[1]);

  bzero(&server_add, sizeof(server_add));

  server_add.sin_family = AF_INET;
  server_add.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_add.sin_port = htons(port_no);

  if(connect(sockfd, &server_add, sizeof(server_add)) < 0) {
    error("ERROR on connect");
  }

  printf("Welcome to online banking management system\n Please enter your login type\n0: admin\n1: normal\n2: joint\n");
  int login;
  scanf("%d\n", &login);
  write(sockfd, buffer, 256);
  // 0: Admin, 1: normal, 2: joint
  if(login == 0) {

  }
  else if(login == 1 || login == 2) {

  }
  else {
    
  }


}
