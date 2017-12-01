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
#include <semaphore.h>
#include "admin.h"
#include "login.h"

void error(char* msg) {
  perror(msg);
  exit(1);
}

int main () {
  int sockfd, new_sock_fd, port_no, clilen;
  char buffer[256];
  struct sockaddr_in server_add, client_add;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd < 0) {
    error("ERROR on opening socket");
  }

  bzero(&server_add, sizeof(server_add));

  port_no = 8887;

  server_add.sin_family = AF_INET;
  server_add.sin_addr.s_addr = INADDR_ANY;
  server_add.sin_port = htons(port_no);

  if (bind(sockfd, (struct sockaddr *) &server_add, sizeof(server_add)) < 0) {
    error("ERROR on binding");
  }
  while(1) {
    listen(sockfd, 10);
    int addrlen = sizeof(client_add);
    new_sock_fd = accept(sockfd, (struct sockaddr *) &client_add, (socklen_t*)&addrlen);
    //printf("---------------\n" );
    if(fork() == 0) {
      printf("In child process\n");
      int login, account_no; // 0: Admin, 1: normal, 2: joint
      char username[10], pin[4], fname[10], lname[10];
      read(new_sock_fd, &login, sizeof(login));
      read(new_sock_fd, username, 10);
      read(new_sock_fd, pin, 4);
      sem_t *sem;
      printf("%d\n", login);
      int result;
      if(login == 0 || login == 1) {
        sem = sem_open(username, IPC_CREAT, 0644, 1);
        sem_wait(sem);
        result = verifyLogin(username, pin, login);
        sem_post(sem);
      }
      else {
        result = verifyLogin(username, pin, login);
      }
      write(sockfd, &result, sizeof(result));
      if(login == 0 && result >= 0) {
        int operation; // 0: add, 1: delete, 2: modify, 3: search;
        read(new_sock_fd, &operation, sizeof(login));

        if(operation == 0) {
          int type; // 0: joint, 1: normal
          read(sockfd, username, sizeof(username));
          read(sockfd, pin, sizeof(pin));
          read(sockfd, fname, sizeof(fname));
          read(sockfd, lname, sizeof(lname));
          if(type == 0) {
            result = addNormalCustomer(username, pin, fname, lname);
            write(sockfd, &result, sizeof(result));
          }
          else if(type == 1) {
            read(sockfd, &account_no, sizeof(account_no));
            result = addJointCustomer(username, pin, fname, lname, account_no);
            write(sockfd, &result, sizeof(result));
          }
        }
        else if(operation == 1) {
          read(sockfd, &account_no, sizeof(account_no));
          result = delete_user(id);
          write(sockfd, &result, sizeof(result));
        }
        else if(operation == 2) {
          int type;
          if(type == 0) {
            //changeFname(id, new);
          }
          else if(type == 1) {
            //changeLname(id, new);
          }
        }
        else if(operation == 3) {

        }
        else {

        }
      }
      else if((login == 1 || login == 2) && result) {
        int operation;
        read(new_sock_fd, &operation, sizeof(login));
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
