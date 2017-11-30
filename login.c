#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <signal.h>
#include <math.h>
#include "admin.h"
#include "customer.h"

//verify login

int verifyLogin(char username[40],char pass[40],int type){
    int id=getId(username);
    if(id==-1) return -1;
    if(verify(id,pass,type)) return id;
    else return -1;
}

 int main(int argc, char *argv[]){
    // if(!init()){
    //     addNormalCustomer("user1","user1","user","1");
    //     addNormalCustomer("user2","user2","user","2");
    //     addNormalCustomer("user3","user3","user","3");
    //     addJointCustomer("user5","user4","user","4",3);
    //     delete(3);
    //     printdataUserId();
    //     printdataUser();
    //     printdataAccount();

    //     //printdataTransactions();
    // }
    // else{
    //     printf("initialized!\n");
    // }
    printf("%d\n",verifyLogin("user2","user2",1));
    printf("%d\n",verifyLogin("admin","admin",0));
    printf("%d\n",verifyLogin("user5","user4",2));
    printf("%d\n",verifyLogin("user6","user1",1));
    printf("%d\n",verifyLogin("user2","user2",2));
    printf("%d\n",verifyLogin("user2","user2",0));
    printf("%d\n",verifyLogin("admin","admin",1));    
 }