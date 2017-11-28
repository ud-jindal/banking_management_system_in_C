  #include <stdio.h>
#include <stdint.h>
#include <sys/resource.h>

int main () {
  int sockfd, new_sock_fd, port_no, clilen;
  char buffer[256];
  struct sockaddr_in server_add, client_add;
  while(true) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
      error("ERROR on opening socket");
    }

    bzero(&server_add, sizeof(server_add));

    port_no = atoi(argv[1]);

    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = INADDR_ANY;
    server_add.sin_port = htons(port_no);

    if (bind(sockfd, (struct sockaddr *) &server_add, sizeof(server_add)) < 0) {
      error("ERROR on binding");
    }

    listen(sockfd, 5);
    int addrlen = sizeof(client_add);
    new_sock_fd = accept(sockfd, (struct sockaddr *) &client_add, (socklen_t*)&addrlen);
    if(fork() == 0) {
      int login; // 0: Admin, 1: normal, 2: joint
      read(new_sock_fd, login, sizeof(login));
      if(login == 0) {
        int operation // 0: add, 1: delete, 2: modify, 3: search;
        read(new_sock_fd, login, sizeof(login));
        if(operation == 0) {

        }
        else if(operation == 1) {

        }
        else if(operation == 2) {

        }
        else if(operation == 3) {

        }
        else {

        }
      }
      else if(login == 1 || login == 2) {
        int operation;
        read(new_sock_fd, login, sizeof(login));
        // Deposit
        if(operation == 0) {

        }
        // Withdraw
        else if(operation == 1) {

        }
        // Balance Enquiry
        else if(operation == 2) {

        }
        // Password Change
        else if(operation == 3) {

        }
        // View details
        else if(operation == 4) {

        }
        // Exit
        else if(operation == 5) {

        }
      }
      else {

      }
    }
  }
}
